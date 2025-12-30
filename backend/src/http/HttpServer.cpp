#include "../../include/http/HttpServer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sstream>
#include <iostream>

using std::string;

HttpServer::HttpServer(Router& r, int p) : router(r), port(p) {}
HttpServer::~HttpServer() {}

int HttpServer::createListenSocket() {
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "socket() failed: " << strerror(errno) << "\n";
        return -1;
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed: " << strerror(errno) << "\n";
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    addr.sin_port = htons(port);

    if (::bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed on port " << port << ": " << strerror(errno) << "\n";
        ::close(sock);
        return -1;
    }
    if (::listen(sock, 16) < 0) {
        std::cerr << "listen() failed: " << strerror(errno) << "\n";
        ::close(sock);
        return -1;
    }
    return sock;
}

void HttpServer::start() {
    int serverSock = createListenSocket();
    if (serverSock < 0) {
        std::cerr << "Failed to start server on port " << port << "\n";
        return;
    }
    std::cout << "HTTP server listening on 0.0.0.0:" << port << "\n";

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &len);
        if (clientSock < 0) continue;
        // Handle client in the same thread (simple, single-threaded)
        handleClient(clientSock);
        ::close(clientSock);
    }
}

static string readAll(int sock) {
    char buffer[8192];
    std::stringstream ss;
    ssize_t n;
    // Read until we think we have the full request headers
    while ((n = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        ss.write(buffer, n);
        // Heuristic: stop if we've seen end of headers and Content-Length satisfied
        string data = ss.str();
        auto pos = data.find("\r\n\r\n");
        if (pos != string::npos) {
            // Check content-length
            auto clPos = data.find("Content-Length:");
            if (clPos != string::npos) {
                auto endLine = data.find("\r\n", clPos);
                string clLine = data.substr(clPos, endLine - clPos);
                int contentLength = 0;
                try {
                    contentLength = std::stoi(clLine.substr(clLine.find(":") + 1));
                } catch (...) { contentLength = 0; }
                size_t bodyStart = pos + 4;
                if (data.size() >= bodyStart + (size_t)contentLength) break;
            } else {
                // No body
                break;
            }
        }
        if (n < (ssize_t)sizeof(buffer)) break;
    }
    return ss.str();
}

void HttpServer::handleClient(int clientSock) {
    string raw = readAll(clientSock);
    HttpRequest req;

    if (!parseRequest(raw, req)) {
        HttpResponse res = HttpResponse::badRequest("Malformed HTTP request");
        addCorsHeaders(res);
        string out = res.toString();
        // Ensure Content-Length
        std::stringstream ss;
        ss << "HTTP/1.1 " << res.getStatusCodeValue() << "\r\n";
        ss << "Content-Length: " << res.getBody().size() << "\r\n";
        ss << "Content-Type: application/json\r\n";
        ss << "Access-Control-Allow-Origin: *\r\n\r\n";
        ss << res.getBody();
        ::send(clientSock, ss.str().c_str(), ss.str().size(), 0);
        return;
    }

    // Preflight CORS
    if (req.getMethod() == HttpMethod::OPTIONS) {
        HttpResponse res(HttpStatus::NO_CONTENT);
        addCorsHeaders(res);
        std::stringstream ss;
        ss << "HTTP/1.1 204\r\n";
        ss << "Access-Control-Allow-Origin: *\r\n";
        ss << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
        ss << "Access-Control-Allow-Headers: Content-Type\r\n";
        ss << "Content-Length: 0\r\n\r\n";
        ::send(clientSock, ss.str().c_str(), ss.str().size(), 0);
        return;
    }

    HttpResponse res = router.handleRequest(req);
    addCorsHeaders(res);

    // Build raw HTTP response with Content-Length
    std::stringstream ss;
    ss << "HTTP/1.1 " << res.getStatusCodeValue() << "\r\n";
    ss << "Content-Length: " << res.getBody().size() << "\r\n";
    ss << "Content-Type: application/json\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n\r\n";
    ss << res.getBody();
    auto out = ss.str();
    ::send(clientSock, out.c_str(), out.size(), 0);
}

bool HttpServer::parseRequest(const string& raw, HttpRequest& outReq) {
    if (raw.empty()) return false;
    std::istringstream ss(raw);
    string line;
    if (!std::getline(ss, line)) return false;
    if (!line.empty() && line.back() == '\r') line.pop_back();

    // Request line: METHOD PATH HTTP/1.1
    std::istringstream rl(line);
    string methodStr, path, httpver;
    rl >> methodStr >> path >> httpver;
    if (methodStr.empty() || path.empty()) return false;

    HttpMethod method = parseMethod(methodStr);
    outReq.setMethod(method);

    // Split path and query
    string urlPath = path;
    auto qpos = path.find('?');
    if (qpos != string::npos) {
        urlPath = path.substr(0, qpos);
        string query = path.substr(qpos + 1);
        std::istringstream qss(query);
        string kv;
        while (std::getline(qss, kv, '&')) {
            auto eq = kv.find('=');
            if (eq != string::npos) {
                string k = urlDecode(kv.substr(0, eq));
                string v = urlDecode(kv.substr(eq + 1));
                outReq.setQueryParam(k, v);
            }
        }
    }
    outReq.setPath(urlPath);

    // Headers
    string headerLine;
    size_t contentLength = 0;
    while (std::getline(ss, headerLine)) {
        if (!headerLine.empty() && headerLine.back() == '\r') headerLine.pop_back();
        if (headerLine.empty()) break; // end of headers
        auto colon = headerLine.find(':');
        if (colon != string::npos) {
            string key = headerLine.substr(0, colon);
            string value = headerLine.substr(colon + 1);
            // trim leading spaces
            value.erase(0, value.find_first_not_of(" \t"));
            outReq.setHeader(key, value);
            if (key == "Content-Length") {
                try { contentLength = static_cast<size_t>(std::stoul(value)); } catch (...) {}
            }
        }
    }

    // Body
    if (contentLength > 0) {
        string body(contentLength, '\0');
        ss.read(&body[0], contentLength);
        outReq.setBody(body);
    }

    return true;
}

HttpMethod HttpServer::parseMethod(const string& m) {
    if (m == "GET") return HttpMethod::GET;
    if (m == "POST") return HttpMethod::POST;
    if (m == "PUT") return HttpMethod::PUT;
    if (m == "DELETE") return HttpMethod::DELETE;
    if (m == "OPTIONS") return HttpMethod::OPTIONS;
    return HttpMethod::GET;
}

string HttpServer::urlDecode(const string& s) {
    string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '%' && i + 2 < s.size()) {
            string hex = s.substr(i + 1, 2);
            char ch = static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            out.push_back(ch);
            i += 2;
        } else if (s[i] == '+') {
            out.push_back(' ');
        } else {
            out.push_back(s[i]);
        }
    }
    return out;
}

void HttpServer::addCorsHeaders(HttpResponse& res) {
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    res.setHeader("Access-Control-Max-Age", "86400");
}

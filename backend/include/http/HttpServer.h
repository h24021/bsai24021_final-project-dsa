#pragma once
#include <string>
#include <thread>
#include <atomic>
#include "HttpModels.h"
#include "../api/Router.h"

class HttpServer {
public:
    HttpServer(Router& router, int port = 8080);
    ~HttpServer();

    void start();

private:
    Router& router;
    int port;

    // Helpers
    int createListenSocket();
    void handleClient(int clientSock);
    bool parseRequest(const std::string& raw, HttpRequest& outReq);
    static HttpMethod parseMethod(const std::string& m);
    static std::string urlDecode(const std::string& s);
    static void addCorsHeaders(HttpResponse& res);
};

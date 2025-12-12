#include "../../include/http/HttpModels.h"
#include <sstream>

HttpRequest::HttpRequest() : method(HttpMethod::GET), path("/") {}

HttpRequest::HttpRequest(HttpMethod m, const string& p) : method(m), path(p) {}

HttpMethod HttpRequest::getMethod() const { return method; }
string HttpRequest::getPath() const { return path; }

string HttpRequest::getHeader(const string& key) const {
    auto it = headers.find(key);
    return (it != headers.end()) ? it->second : "";
}

string HttpRequest::getQueryParam(const string& key) const {
    auto it = queryParams.find(key);
    return (it != queryParams.end()) ? it->second : "";
}

string HttpRequest::getPathParam(const string& key) const {
    auto it = pathParams.find(key);
    return (it != pathParams.end()) ? it->second : "";
}

string HttpRequest::getBody() const { return body; }

void HttpRequest::setMethod(HttpMethod m) { method = m; }
void HttpRequest::setPath(const string& p) { path = p; }
void HttpRequest::setHeader(const string& key, const string& value) { headers[key] = value; }
void HttpRequest::setQueryParam(const string& key, const string& value) { queryParams[key] = value; }
void HttpRequest::setPathParam(const string& key, const string& value) { pathParams[key] = value; }
void HttpRequest::setBody(const string& b) { body = b; }

bool HttpRequest::hasQueryParam(const string& key) const {
    return queryParams.find(key) != queryParams.end();
}

bool HttpRequest::hasPathParam(const string& key) const {
    return pathParams.find(key) != pathParams.end();
}

map<string, string> HttpRequest::getAllQueryParams() const {
    return queryParams;
}

HttpResponse::HttpResponse() : statusCode(HttpStatus::OK) {
    setContentType("application/json");
}

HttpResponse::HttpResponse(HttpStatus status) : statusCode(status) {
    setContentType("application/json");
}

HttpStatus HttpResponse::getStatusCode() const { return statusCode; }

int HttpResponse::getStatusCodeValue() const {
    return static_cast<int>(statusCode);
}

string HttpResponse::getHeader(const string& key) const {
    auto it = headers.find(key);
    return (it != headers.end()) ? it->second : "";
}

string HttpResponse::getBody() const { return body; }

void HttpResponse::setStatusCode(HttpStatus status) { statusCode = status; }
void HttpResponse::setHeader(const string& key, const string& value) { headers[key] = value; }
void HttpResponse::setBody(const string& b) { body = b; }

void HttpResponse::setJsonBody(const string& json) {
    setContentType("application/json");
    setBody(json);
}

void HttpResponse::setContentType(const string& type) {
    setHeader("Content-Type", type);
}

HttpResponse HttpResponse::ok(const string& body) {
    HttpResponse response(HttpStatus::OK);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::created(const string& body) {
    HttpResponse response(HttpStatus::CREATED);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::badRequest(const string& message) {
    HttpResponse response(HttpStatus::BAD_REQUEST);
    response.setBody(JsonHelper::createErrorResponse(message, "BAD_REQUEST"));
    return response;
}

HttpResponse HttpResponse::notFound(const string& message) {
    HttpResponse response(HttpStatus::NOT_FOUND);
    response.setBody(JsonHelper::createErrorResponse(message, "NOT_FOUND"));
    return response;
}

HttpResponse HttpResponse::serverError(const string& message) {
    HttpResponse response(HttpStatus::INTERNAL_SERVER_ERROR);
    response.setBody(JsonHelper::createErrorResponse(message, "SERVER_ERROR"));
    return response;
}

string HttpResponse::toString() const {
    stringstream ss;
    ss << "HTTP/1.1 " << getStatusCodeValue() << "\r\n";

    for (const auto& header : headers) {
        ss << header.first << ": " << header.second << "\r\n";
    }

    ss << "\r\n" << body;
    return ss.str();
}

string JsonHelper::createSuccessResponse(const string& data, const string& message) {
    stringstream ss;
    ss << "{";
    ss << "\"status\":\"success\"";

    if (!message.empty()) {
        ss << ",\"message\":\"" << escapeJson(message) << "\"";
    }

    if (!data.empty()) {
        ss << ",\"data\":" << data;
    }

    ss << "}";
    return ss.str();
}

string JsonHelper::createErrorResponse(const string& message, const string& code) {
    stringstream ss;
    ss << "{";
    ss << "\"status\":\"error\"";
    ss << ",\"message\":\"" << escapeJson(message) << "\"";
    ss << ",\"code\":\"" << code << "\"";
    ss << "}";
    return ss.str();
}

string JsonHelper::createObject(const map<string, string>& fields) {
    stringstream ss;
    ss << "{";

    bool first = true;
    for (const auto& field : fields) {
        if (!first) ss << ",";
        ss << "\"" << escapeJson(field.first) << "\":";

        if (!field.second.empty() &&
            (isdigit(field.second[0]) || field.second == "true" || field.second == "false" ||
             field.second[0] == '[' || field.second[0] == '{')) {
            ss << field.second;
        } else {
            ss << "\"" << escapeJson(field.second) << "\"";
        }

        first = false;
    }

    ss << "}";
    return ss.str();
}

string JsonHelper::createArray(const vector<string>& items) {
    stringstream ss;
    ss << "[";

    for (size_t i = 0; i < items.size(); i++) {
        if (i > 0) ss << ",";
        ss << items[i];
    }

    ss << "]";
    return ss.str();
}

string JsonHelper::escapeJson(const string& str) {
    stringstream ss;
    for (char c : str) {
        switch (c) {
            case '"':  ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:   ss << c; break;
        }
    }
    return ss.str();
}

map<string, string> JsonHelper::parseSimpleJson(const string& json) {
    map<string, string> result;

    size_t pos = 0;
    while (pos < json.length()) {

        size_t keyStart = json.find('"', pos);
        if (keyStart == string::npos) break;

        size_t keyEnd = json.find('"', keyStart + 1);
        if (keyEnd == string::npos) break;

        string key = json.substr(keyStart + 1, keyEnd - keyStart - 1);

        size_t colon = json.find(':', keyEnd);
        if (colon == string::npos) break;

        size_t valueStart = json.find_first_not_of(" \t\n\r", colon + 1);
        if (valueStart == string::npos) break;

        string value;
        if (json[valueStart] == '"') {

            size_t valueEnd = json.find('"', valueStart + 1);
            if (valueEnd == string::npos) break;
            value = json.substr(valueStart + 1, valueEnd - valueStart - 1);
            pos = valueEnd + 1;
        } else {

            size_t valueEnd = json.find_first_of(",}", valueStart);
            if (valueEnd == string::npos) valueEnd = json.length();
            value = json.substr(valueStart, valueEnd - valueStart);

            size_t end = value.find_last_not_of(" \t\n\r");
            if (end != string::npos) value = value.substr(0, end + 1);
            pos = valueEnd;
        }

        result[key] = value;
    }

    return result;
}

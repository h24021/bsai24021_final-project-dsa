#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    OPTIONS
};

enum class HttpStatus {
    OK = 200,
    CREATED = 201,
    NO_CONTENT = 204,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    CONFLICT = 409,
    INTERNAL_SERVER_ERROR = 500
};

class HttpRequest {
private:
    HttpMethod method;
    string path;
    map<string, string> headers;
    map<string, string> queryParams;
    map<string, string> pathParams;
    string body;

public:
    HttpRequest();
    HttpRequest(HttpMethod m, const string& p);

    HttpMethod getMethod() const;
    string getPath() const;
    string getHeader(const string& key) const;
    string getQueryParam(const string& key) const;
    string getPathParam(const string& key) const;
    string getBody() const;

    void setMethod(HttpMethod m);
    void setPath(const string& p);
    void setHeader(const string& key, const string& value);
    void setQueryParam(const string& key, const string& value);
    void setPathParam(const string& key, const string& value);
    void setBody(const string& b);

    bool hasQueryParam(const string& key) const;
    bool hasPathParam(const string& key) const;
    map<string, string> getAllQueryParams() const;
};

class HttpResponse {
private:
    HttpStatus statusCode;
    map<string, string> headers;
    string body;

public:
    HttpResponse();
    HttpResponse(HttpStatus status);

    HttpStatus getStatusCode() const;
    int getStatusCodeValue() const;
    string getHeader(const string& key) const;
    string getBody() const;

    void setStatusCode(HttpStatus status);
    void setHeader(const string& key, const string& value);
    void setBody(const string& b);

    void setJsonBody(const string& json);
    void setContentType(const string& type);

    static HttpResponse ok(const string& body = "");
    static HttpResponse created(const string& body = "");
    static HttpResponse badRequest(const string& message);
    static HttpResponse notFound(const string& message);
    static HttpResponse serverError(const string& message);

    string toString() const;
};

class JsonHelper {
public:

    static string createSuccessResponse(const string& data, const string& message = "");
    static string createErrorResponse(const string& message, const string& code = "ERROR");
    static string createObject(const map<string, string>& fields);
    static string createArray(const vector<string>& items);

    static string escapeJson(const string& str);

    static map<string, string> parseSimpleJson(const string& json);
};

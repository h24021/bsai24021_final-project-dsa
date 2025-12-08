#pragma once
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <regex>
#include "../http/HttpModels.h"

using namespace std;

using RouteHandler = function<HttpResponse(const HttpRequest&)>;

struct Route {
    HttpMethod method;
    string path;
    regex pathPattern;
    vector<string> paramNames;
    RouteHandler handler;
    
    Route(HttpMethod m, const string& p, RouteHandler h);
    bool matches(HttpMethod m, const string& requestPath) const;
    void extractParams(const string& requestPath, HttpRequest& request) const;
};

class Router {
private:
    vector<Route> routes;
    string basePath;

    regex createPathPattern(const string& path, vector<string>& paramNames);
    string normalizePath(const string& path) const;

public:
    Router(const string& base = "/api/v1");

    void get(const string& path, RouteHandler handler);
    void post(const string& path, RouteHandler handler);
    void put(const string& path, RouteHandler handler);
    void del(const string& path, RouteHandler handler);   
    void registerRoute(HttpMethod method, const string& path, RouteHandler handler);

    HttpResponse handleRequest(const HttpRequest& request);
    HttpResponse routeRequest(HttpMethod method, const string& path, const HttpRequest& request);

    void printRoutes() const;
    int getRouteCount() const;
    string getBasePath() const;

    static HttpRequest createRequest(HttpMethod method, const string& path, const string& body = "");
};

#pragma once
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <regex>
#include "../http/HttpModels.h"

using namespace std;

// Route handler type
using RouteHandler = function<HttpResponse(const HttpRequest&)>;

// Route structure
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

// Router class
class Router {
private:
    vector<Route> routes;
    string basePath;
    
    // Helper methods
    regex createPathPattern(const string& path, vector<string>& paramNames);
    string normalizePath(const string& path) const;

public:
    Router(const string& base = "/api/v1");
    
    // Route registration methods
    void get(const string& path, RouteHandler handler);
    void post(const string& path, RouteHandler handler);
    void put(const string& path, RouteHandler handler);
    void del(const string& path, RouteHandler handler);  // 'delete' is a keyword
    void registerRoute(HttpMethod method, const string& path, RouteHandler handler);
    
    // Request handling
    HttpResponse handleRequest(const HttpRequest& request);
    HttpResponse routeRequest(HttpMethod method, const string& path, const HttpRequest& request);
    
    // Utility methods
    void printRoutes() const;
    int getRouteCount() const;
    string getBasePath() const;
    
    // Static helper to create requests (for testing)
    static HttpRequest createRequest(HttpMethod method, const string& path, const string& body = "");
};

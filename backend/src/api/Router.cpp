#include "../../include/api/Router.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// ============== Route Implementation ==============

Route::Route(HttpMethod m, const string& p, RouteHandler h) 
    : method(m), path(p), handler(h) {
    // Convert path to regex pattern and extract parameter names
    string pattern = p;
    size_t pos = 0;
    
    while ((pos = pattern.find(':', pos)) != string::npos) {
        size_t end = pattern.find('/', pos);
        if (end == string::npos) end = pattern.length();
        
        string paramName = pattern.substr(pos + 1, end - pos - 1);
        paramNames.push_back(paramName);
        
        // Replace :param with regex capture group
        pattern.replace(pos, end - pos, "([^/]+)");
        pos = end;
    }
    
    // Create regex from pattern
    pathPattern = regex("^" + pattern + "$");
}

bool Route::matches(HttpMethod m, const string& requestPath) const {
    if (m != method) return false;
    return regex_match(requestPath, pathPattern);
}

void Route::extractParams(const string& requestPath, HttpRequest& request) const {
    smatch matches;
    if (regex_match(requestPath, matches, pathPattern)) {
        for (size_t i = 0; i < paramNames.size() && i + 1 < matches.size(); i++) {
            request.setPathParam(paramNames[i], matches[i + 1].str());
        }
    }
}

// ============== Router Implementation ==============

Router::Router(const string& base) : basePath(base) {}

regex Router::createPathPattern(const string& path, vector<string>& paramNames) {
    string pattern = path;
    size_t pos = 0;
    
    while ((pos = pattern.find(':', pos)) != string::npos) {
        size_t end = pattern.find('/', pos);
        if (end == string::npos) end = pattern.length();
        
        string paramName = pattern.substr(pos + 1, end - pos - 1);
        paramNames.push_back(paramName);
        
        pattern.replace(pos, end - pos, "([^/]+)");
        pos = end;
    }
    
    return regex("^" + pattern + "$");
}

string Router::normalizePath(const string& path) const {
    string normalized = path;
    
    // Remove trailing slash
    if (normalized.length() > 1 && normalized.back() == '/') {
        normalized.pop_back();
    }
    
    // Ensure leading slash
    if (normalized.empty() || normalized[0] != '/') {
        normalized = "/" + normalized;
    }
    
    return normalized;
}

void Router::get(const string& path, RouteHandler handler) {
    registerRoute(HttpMethod::GET, path, handler);
}

void Router::post(const string& path, RouteHandler handler) {
    registerRoute(HttpMethod::POST, path, handler);
}

void Router::put(const string& path, RouteHandler handler) {
    registerRoute(HttpMethod::PUT, path, handler);
}

void Router::del(const string& path, RouteHandler handler) {
    registerRoute(HttpMethod::DELETE, path, handler);
}

void Router::registerRoute(HttpMethod method, const string& path, RouteHandler handler) {
    string fullPath = basePath + normalizePath(path);
    routes.push_back(Route(method, fullPath, handler));
}

HttpResponse Router::handleRequest(const HttpRequest& request) {
    return routeRequest(request.getMethod(), request.getPath(), request);
}

HttpResponse Router::routeRequest(HttpMethod method, const string& path, const HttpRequest& request) {
    string normalizedPath = normalizePath(path);
    
    // Try to find matching route
    for (auto& route : routes) {
        if (route.matches(method, normalizedPath)) {
            // Create a copy of request to add path params
            HttpRequest modifiedRequest = request;
            route.extractParams(normalizedPath, modifiedRequest);
            
            // Call the handler
            try {
                return route.handler(modifiedRequest);
            } catch (const exception& e) {
                return HttpResponse::serverError(string("Handler error: ") + e.what());
            }
        }
    }
    
    // No route found
    return HttpResponse::notFound("Route not found: " + normalizedPath);
}

void Router::printRoutes() const {
    cout << "\n========== Registered API Routes ==========\n";
    cout << "Base Path: " << basePath << "\n\n";
    
    map<string, vector<string>> methodGroups;
    
    for (const auto& route : routes) {
        string methodStr;
        switch (route.method) {
            case HttpMethod::GET: methodStr = "GET"; break;
            case HttpMethod::POST: methodStr = "POST"; break;
            case HttpMethod::PUT: methodStr = "PUT"; break;
            case HttpMethod::DELETE: methodStr = "DELETE"; break;
            default: methodStr = "UNKNOWN"; break;
        }
        
        methodGroups[methodStr].push_back(route.path);
    }
    
    for (const auto& group : methodGroups) {
        cout << group.first << ":\n";
        for (const auto& path : group.second) {
            cout << "  " << path << "\n";
        }
        cout << "\n";
    }
    
    cout << "Total Routes: " << routes.size() << "\n";
    cout << "==========================================\n\n";
}

int Router::getRouteCount() const {
    return routes.size();
}

string Router::getBasePath() const {
    return basePath;
}

HttpRequest Router::createRequest(HttpMethod method, const string& path, const string& body) {
    HttpRequest request(method, path);
    request.setBody(body);
    return request;
}


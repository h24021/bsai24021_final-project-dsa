#include <iostream>
#include <string>
#include <map>
#include <functional>

using namespace std;

/**
 * Router - Simple routing mechanism for API endpoints
 * This simulates a REST API router that will later integrate with a web framework
 * (like cpp-httplib, Crow, or similar)
 */
class Router {
private:
    map<string, function<string(string)>> routes;

public:
    void registerRoute(string method, string path, function<string(string)> handler) {
        string key = method + " " + path;
        routes[key] = handler;
    }

    string handleRequest(string method, string path, string body = "") {
        string key = method + " " + path;
        if (routes.find(key) != routes.end()) {
            return routes[key](body);
        }
        return "{\"status\":\"error\",\"code\":404,\"message\":\"Route not found\"}";
    }

    void printRoutes() {
        cout << "Registered API Routes:" << endl;
        for (const auto& route : routes) {
            cout << "  " << route.first << endl;
        }
    }
};

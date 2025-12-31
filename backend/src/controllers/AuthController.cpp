#include "../../include/controllers/AuthController.h"
#include "../../include/http/HttpModels.h"
#include <sstream>

AuthController::AuthController(Library* lib, SessionManager* sm) 
    : library(lib), sessionManager(sm) {}

string AuthController::extractToken(const HttpRequest& request) {
    string authHeader = request.getHeader("Authorization");
    if (authHeader.empty()) {
        return "";
    }
    
    // Format: "Bearer <token>"
    if (authHeader.find("Bearer ") == 0) {
        return authHeader.substr(7);  // Skip "Bearer "
    }
    
    return "";
}

bool AuthController::validateAdminKey(const string& key) {
    return key == ADMIN_KEY;
}

HttpResponse AuthController::login(const HttpRequest& request) {
    string body = request.getBody();
    
    // Parse JSON body manually (simple parsing)
    string username_or_email = "";
    string password = "";
    string admin_key = "";
    
    // Extract username_or_email
    size_t pos = body.find("\"username_or_email\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 20);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            username_or_email = body.substr(start + 1, end - start - 1);
        }
    }
    
    // Extract password
    pos = body.find("\"password\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 11);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            password = body.substr(start + 1, end - start - 1);
        }
    }
    
    // Extract admin_key (optional)
    pos = body.find("\"admin_key\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 12);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            admin_key = body.substr(start + 1, end - start - 1);
        }
    }
    
    if (username_or_email.empty() || password.empty()) {
        return HttpResponse::badRequest("{\"error\": \"Missing username/email or password\"}");
    }
    
    // Find user by username or email
    User* user = library->findUserByUsername(username_or_email);
    if (!user) {
        user = library->findUserByEmail(username_or_email);
    }
    
    if (!user) {
        return HttpResponse::badRequest("{\"error\": \"Invalid credentials\"}");
    }
    
    // Validate password
    if (user->getPassword() != password) {
        return HttpResponse::badRequest("{\"error\": \"Invalid credentials\"}");
    }
    
    // Determine role
    string role = "USER";
    if (!admin_key.empty() && validateAdminKey(admin_key)) {
        role = "ADMIN";
    }
    
    // Create session
    string token = sessionManager->createSession(user->getUserID(), user->getUsername(), role);
    
    // Build response
    stringstream response;
    response << "{"
             << "\"token\": \"" << JsonHelper::escapeJson(token) << "\","
             << "\"user\": {"
             << "\"userID\": " << user->getUserID() << ","
             << "\"username\": \"" << JsonHelper::escapeJson(user->getUsername()) << "\","
             << "\"name\": \"" << JsonHelper::escapeJson(user->getName()) << "\","
             << "\"email\": \"" << JsonHelper::escapeJson(user->getEmail()) << "\""
             << "},"
             << "\"role\": \"" << role << "\""
             << "}";
    
    HttpResponse resp = HttpResponse::ok(response.str());
    resp.setContentType("application/json");
    return resp;
}

HttpResponse AuthController::registerUser(const HttpRequest& request) {
    string body = request.getBody();
    
    // Parse JSON body
    string username = "";
    string name = "";
    string email = "";
    string password = "";
    string admin_key = "";
    
    // Extract username
    size_t pos = body.find("\"username\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 11);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            username = body.substr(start + 1, end - start - 1);
        }
    }
    
    // Extract name
    pos = body.find("\"name\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 7);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            name = body.substr(start + 1, end - start - 1);
        }
    }
    
    // Extract email
    pos = body.find("\"email\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 8);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            email = body.substr(start + 1, end - start - 1);
        }
    }
    
    // Extract password
    pos = body.find("\"password\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 11);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            password = body.substr(start + 1, end - start - 1);
        }
    }
    
    // Extract admin_key (optional)
    pos = body.find("\"admin_key\"");
    if (pos != string::npos) {
        size_t start = body.find("\"", pos + 12);
        size_t end = body.find("\"", start + 1);
        if (start != string::npos && end != string::npos) {
            admin_key = body.substr(start + 1, end - start - 1);
        }
    }
    
    if (username.empty() || name.empty() || email.empty() || password.empty()) {
        return HttpResponse::badRequest("{\"error\": \"Missing required fields\"}");
    }
    
    // Check if username or email already exists
    if (library->findUserByUsername(username)) {
        return HttpResponse::badRequest("{\"error\": \"Username already exists\"}");
    }
    
    if (library->findUserByEmail(email)) {
        return HttpResponse::badRequest("{\"error\": \"Email already exists\"}");
    }
    
    // Determine role
    string role = "USER";
    if (!admin_key.empty() && validateAdminKey(admin_key)) {
        role = "ADMIN";
    }
    
    // Generate new user ID
    int newUserID = library->getTotalUsers() + 1;
    
    // Create and add user
    User newUser(newUserID, username, name, email, password, role);
    library->addUser(newUser);
    
    // Build response
    stringstream response;
    response << "{"
             << "\"user\": {"
             << "\"userID\": " << newUserID << ","
             << "\"username\": \"" << JsonHelper::escapeJson(username) << "\","
             << "\"name\": \"" << JsonHelper::escapeJson(name) << "\","
             << "\"email\": \"" << JsonHelper::escapeJson(email) << "\","
             << "\"role\": \"" << role << "\""
             << "},"
             << "\"message\": \"User registered successfully\""
             << "}";
    
    HttpResponse resp = HttpResponse::created(response.str());
    resp.setContentType("application/json");
    return resp;
}

HttpResponse AuthController::logout(const HttpRequest& request) {
    string token = extractToken(request);
    
    if (token.empty()) {
        return HttpResponse::badRequest("{\"error\": \"No token provided\"}");
    }
    
    bool removed = sessionManager->removeSession(token);
    
    if (removed) {
        return HttpResponse::ok("{\"message\": \"Logged out successfully\"}");
    } else {
        return HttpResponse::badRequest("{\"error\": \"Invalid or expired token\"}");
    }
}

HttpResponse AuthController::getCurrentUser(const HttpRequest& request) {
    string token = extractToken(request);
    
    if (token.empty()) {
        HttpResponse resp(HttpStatus::UNAUTHORIZED);
        resp.setJsonBody("{\"error\": \"No token provided\"}");
        return resp;
    }
    
    Session* session = sessionManager->validateSession(token);
    
    if (!session) {
        HttpResponse resp(HttpStatus::UNAUTHORIZED);
        resp.setJsonBody("{\"error\": \"Invalid or expired token\"}");
        return resp;
    }
    
    User* user = library->findUserByID(session->userID);
    
    if (!user) {
        return HttpResponse::notFound("{\"error\": \"User not found\"}");
    }
    
    // Build response
    stringstream response;
    response << "{"
             << "\"user\": {"
             << "\"userID\": " << user->getUserID() << ","
             << "\"username\": \"" << JsonHelper::escapeJson(user->getUsername()) << "\","
             << "\"name\": \"" << JsonHelper::escapeJson(user->getName()) << "\","
             << "\"email\": \"" << JsonHelper::escapeJson(user->getEmail()) << "\","
             << "\"role\": \"" << session->role << "\""
             << "}"
             << "}";
    
    HttpResponse resp = HttpResponse::ok(response.str());
    resp.setContentType("application/json");
    return resp;
}

#pragma once
#include <string>
#include <map>
#include <ctime>
#include "../models/User.h"

using namespace std;

struct Session {
    int userID;
    string username;
    string role;
    time_t createdAt;
    time_t expiresAt;
};

class SessionManager {
private:
    map<string, Session> sessions;  // token -> Session
    static const int SESSION_DURATION = 86400; // 24 hours in seconds

    string generateToken();
    bool isExpired(const Session& session);

public:
    SessionManager();
    
    // Create new session, returns session token
    string createSession(int userID, const string& username, const string& role);
    
    // Validate session token, returns Session if valid, nullptr otherwise
    Session* validateSession(const string& token);
    
    // Remove session (logout)
    bool removeSession(const string& token);
    
    // Get user ID from token
    int getUserIDFromToken(const string& token);
    
    // Get user role from token
    string getRoleFromToken(const string& token);
    
    // Check if user is admin
    bool isAdmin(const string& token);
    
    // Clean expired sessions
    void cleanExpiredSessions();
};
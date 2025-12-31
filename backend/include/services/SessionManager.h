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
    map<string, Session> sessions;  
    static const int SESSION_DURATION = 86400; 

    string generateToken();
    bool isExpired(const Session& session);

public:
    SessionManager();
    
    string createSession(int userID, const string& username, const string& role);
    
    Session* validateSession(const string& token);
    
    bool removeSession(const string& token);
    
    int getUserIDFromToken(const string& token);
    
    string getRoleFromToken(const string& token);
    
    bool isAdmin(const string& token);
    
    void cleanExpiredSessions();
};
#include "../../include/services/SessionManager.h"
#include <random>
#include <sstream>
#include <iomanip>

SessionManager::SessionManager() {}

string SessionManager::generateToken() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    stringstream ss;
    ss << hex << setfill('0');
    
    ss << time(nullptr) << "_";
    
    for (int i = 0; i < 32; i++) {
        ss << setw(2) << dis(gen);
    }
    
    return ss.str();
}

bool SessionManager::isExpired(const Session& session) {
    return time(nullptr) >= session.expiresAt;
}

string SessionManager::createSession(int userID, const string& username, const string& role) {
    string token = generateToken();
    time_t now = time(nullptr);
    
    Session session;
    session.userID = userID;
    session.username = username;
    session.role = role;
    session.createdAt = now;
    session.expiresAt = now + SESSION_DURATION;
    
    sessions[token] = session;
    
    return token;
}

Session* SessionManager::validateSession(const string& token) {
    auto it = sessions.find(token);
    
    if (it == sessions.end()) {
        return nullptr; 
    }
    
    if (isExpired(it->second)) {
        sessions.erase(it);
        return nullptr; 
    }
    
    return &(it->second);
}

bool SessionManager::removeSession(const string& token) {
    auto it = sessions.find(token);
    if (it != sessions.end()) {
        sessions.erase(it);
        return true;
    }
    return false;
}

int SessionManager::getUserIDFromToken(const string& token) {
    Session* session = validateSession(token);
    if (session) {
        return session->userID;
    }
    return -1; 
}

string SessionManager::getRoleFromToken(const string& token) {
    Session* session = validateSession(token);
    if (session) {
        return session->role;
    }
    return ""; 
}

bool SessionManager::isAdmin(const string& token) {
    Session* session = validateSession(token);
    if (session) {
        return session->role == "ADMIN";
    }
    return false;
}

void SessionManager::cleanExpiredSessions() {
    auto it = sessions.begin();
    while (it != sessions.end()) {
        if (isExpired(it->second)) {
            it = sessions.erase(it);
        } else {
            ++it;
        }
    }
}

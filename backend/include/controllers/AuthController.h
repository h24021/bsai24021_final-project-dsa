#pragma once
#include <string>
#include "../http/HttpModels.h"
#include "../services/Library.h"
#include "../services/SessionManager.h"

using namespace std;

class AuthController {
private:
    Library* library;
    SessionManager* sessionManager;
    const string ADMIN_KEY = "hamna"; 
public:
    AuthController(Library* lib, SessionManager* sm);
    
  
    HttpResponse login(const HttpRequest& request);
    
    
    HttpResponse registerUser(const HttpRequest& request);
    
   
    HttpResponse logout(const HttpRequest& request);
    
    
    HttpResponse getCurrentUser(const HttpRequest& request);
    
    string extractToken(const HttpRequest& request);
    bool validateAdminKey(const string& key);
};

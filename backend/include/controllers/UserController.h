#pragma once
#include <string>
#include <vector>
#include "../models/User.h"
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

class UserController {
private:
    Library* library;

    string userToJson(const User& user);

public:
    UserController(Library* lib);

    HttpResponse getAllUsers(const HttpRequest& req);            
    HttpResponse getUserById(const HttpRequest& req);            
    HttpResponse getUserByEmail(const HttpRequest& req);         
    HttpResponse createUser(const HttpRequest& req);             
    HttpResponse updateUser(const HttpRequest& req);             
    HttpResponse deleteUser(const HttpRequest& req);             
    HttpResponse getBorrowedBooks(const HttpRequest& req);       
};

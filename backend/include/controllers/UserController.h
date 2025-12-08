#pragma once
#include <string>
#include <vector>
#include "../models/User.h"
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

/**
 * UserController - Handles REST API requests for user operations
 * This is the bridge between HTTP endpoints and business logic
 */
class UserController {
private:
    Library* library;
    
    // Helper method to convert User to JSON string
    string userToJson(const User& user);

public:
    UserController(Library* lib);

    // REST API endpoint handlers
    HttpResponse getAllUsers(const HttpRequest& req);           // GET /users
    HttpResponse getUserById(const HttpRequest& req);           // GET /users/:id
    HttpResponse getUserByEmail(const HttpRequest& req);        // GET /users/email/:email
    HttpResponse createUser(const HttpRequest& req);            // POST /users
    HttpResponse updateUser(const HttpRequest& req);            // PUT /users/:id
    HttpResponse deleteUser(const HttpRequest& req);            // DELETE /users/:id
    HttpResponse getBorrowedBooks(const HttpRequest& req);      // GET /users/:id/borrowed
};

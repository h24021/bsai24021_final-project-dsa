#pragma once
#include <string>
#include <vector>
#include "../models/User.h"
#include "../services/Library.h"

using namespace std;

/**
 * UserController - Handles HTTP-like requests for user operations
 * This will be the bridge between API endpoints and business logic
 */
class UserController {
private:
    Library* library;

public:
    UserController(Library* lib);

    // REST-like methods that will be exposed via API
    string getAllUsers();           // GET /api/users
    string getUserById(int id);     // GET /api/users/:id
    string createUser(int id, string name, string role); // POST /api/users
    string updateUser(int id, string name, string role); // PUT /api/users/:id
    string deleteUser(int id);      // DELETE /api/users/:id
    
    // Borrowing operations
    string borrowBook(int userId, int bookId);    // POST /api/users/:id/borrow
    string returnBook(int userId, int bookId);    // POST /api/users/:id/return
    
    // JSON response helpers
    string toJSON(const User& user);
    string errorResponse(string message, int code);
    string successResponse(string message);
};

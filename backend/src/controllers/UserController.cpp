#include "../../include/controllers/UserController.h"
#include <sstream>

UserController::UserController(Library* lib) : library(lib) {}

string UserController::getAllUsers() {
    // TODO: Implement fetching all users and return as JSON array
    return "{\"status\":\"success\",\"data\":[]}";
}

string UserController::getUserById(int id) {
    // TODO: Implement search by ID and return JSON
    return "{\"status\":\"success\",\"data\":null}";
}

string UserController::createUser(int id, string name, string role) {
    User newUser(id, name, role);
    library->addUser(newUser);
    return successResponse("User created successfully");
}

string UserController::updateUser(int id, string name, string role) {
    // TODO: Implement user update logic
    return successResponse("User updated successfully");
}

string UserController::deleteUser(int id) {
    // TODO: Implement user deletion logic
    return successResponse("User deleted successfully");
}

string UserController::borrowBook(int userId, int bookId) {
    // TODO: Implement borrow logic
    return successResponse("Book borrowed successfully");
}

string UserController::returnBook(int userId, int bookId) {
    // TODO: Implement return logic
    return successResponse("Book returned successfully");
}

string UserController::toJSON(const User& user) {
    stringstream ss;
    ss << "{"
       << "\"userID\":" << user.getUserID()
       << "}";
    return ss.str();
}

string UserController::errorResponse(string message, int code) {
    stringstream ss;
    ss << "{\"status\":\"error\",\"code\":" << code 
       << ",\"message\":\"" << message << "\"}";
    return ss.str();
}

string UserController::successResponse(string message) {
    return "{\"status\":\"success\",\"message\":\"" + message + "\"}";
}

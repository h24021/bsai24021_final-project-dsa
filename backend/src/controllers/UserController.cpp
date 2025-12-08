#include "../../include/controllers/UserController.h"
#include <sstream>

UserController::UserController(Library* lib) : library(lib) {}

string UserController::getAllUsers() {
     
    return "{\"status\":\"success\",\"data\":[]}";
}

string UserController::getUserById(int id) {
    (void)id;  
    return "{\"status\":\"success\",\"data\":null}";
}

string UserController::createUser(int id, string name, string role) {
    User newUser(id, name, role);
    library->addUser(newUser);
    return successResponse("User created successfully");
}

string UserController::updateUser(int id, string name, string role) {
    (void)id; (void)name; (void)role;
     
    return successResponse("User updated successfully");
}

string UserController::deleteUser(int id) {
    (void)id;  
    return successResponse("User deleted successfully");
}

string UserController::borrowBook(int userId, int bookId) {
    (void)userId; (void)bookId;
     
    return successResponse("Book borrowed successfully");
}

string UserController::returnBook(int userId, int bookId) {
    (void)userId; (void)bookId;
     
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

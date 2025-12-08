#include "../../include/controllers/UserController.h"
#include "../../include/http/HttpModels.h"
#include <sstream>
#include <vector>

UserController::UserController(Library* lib) : library(lib) {}

// Helper: Convert User object to JSON string
string UserController::userToJson(const User& user) {
    stringstream ss;
    ss << "{";
    ss << "\"userID\":" << user.getUserID() << ",";
    ss << "\"name\":\"" << JsonHelper::escapeJson(user.getName()) << "\",";
    ss << "\"email\":\"" << JsonHelper::escapeJson(user.getEmail()) << "\",";
    ss << "\"role\":\"" << JsonHelper::escapeJson(user.getRole()) << "\",";
    
    // Borrowed books array
    ss << "\"borrowedBooks\":[";
    const auto& borrowedBooks = user.getBorrowedBookIDs();
    for (size_t i = 0; i < borrowedBooks.size(); i++) {
        ss << borrowedBooks[i];
        if (i < borrowedBooks.size() - 1) ss << ",";
    }
    ss << "]";
    
    ss << "}";
    return ss.str();
}

// GET /users - Get all users
HttpResponse UserController::getAllUsers(const HttpRequest& req) {
    (void)req; // No query params needed for this endpoint
    
    try {
        vector<User> users = library->getAllUsers();
        
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < users.size(); i++) {
            ss << userToJson(users[i]);
            if (i < users.size() - 1) ss << ",";
        }
        ss << "]";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(ss.str(), "Retrieved " + to_string(users.size()) + " users")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve users: " + string(e.what()))
        );
    }
}

// GET /users/:id - Get user by ID
HttpResponse UserController::getUserById(const HttpRequest& req) {
    try {
        string idStr = req.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("User ID is required")
            );
        }
        
        int userId = stoi(idStr);
        User* user = library->findUserByID(userId);
        
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))
            );
        }
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(userToJson(*user))
        );
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid user ID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve user: " + string(e.what()))
        );
    }
}

// GET /users/email/:email - Get user by email
HttpResponse UserController::getUserByEmail(const HttpRequest& req) {
    try {
        string email = req.getPathParam("email");
        if (email.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("Email is required")
            );
        }
        
        User* user = library->findUserByEmail(email);
        
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with email: " + email)
            );
        }
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(userToJson(*user))
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve user: " + string(e.what()))
        );
    }
}

// POST /users - Create new user
HttpResponse UserController::createUser(const HttpRequest& req) {
    try {
        // Parse JSON body
        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());
        
        string name = body["name"];
        string email = body["email"];
        string role = body.count("role") ? body["role"] : "member";
        
        if (name.empty() || email.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("Name and email are required")
            );
        }
        
        // Check if email already exists
        User* existingUser = library->findUserByEmail(email);
        if (existingUser) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("User with this email already exists")
            );
        }
        
        // Generate new user ID (simple auto-increment starting from 5000)
        static int nextUserID = 5000;
        int newUserID = nextUserID++;
        
        // Create and add user (constructor takes: id, name, email, role)
        User newUser(newUserID, name, email, role);
        library->addUser(newUser);
        
        return HttpResponse::created(
            JsonHelper::createSuccessResponse(
                userToJson(newUser),
                "User created successfully"
            )
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to create user: " + string(e.what()))
        );
    }
}

// PUT /users/:id - Update user
HttpResponse UserController::updateUser(const HttpRequest& req) {
    try {
        string idStr = req.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("User ID is required")
            );
        }
        
        int userId = stoi(idStr);
        User* user = library->findUserByID(userId);
        
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))
            );
        }
        
        // TODO: Implement update logic (requires setters in User class)
        // For now, return success with current user data
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                userToJson(*user),
                "User update not yet implemented"
            )
        );
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid user ID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to update user: " + string(e.what()))
        );
    }
}

// DELETE /users/:id - Delete user
HttpResponse UserController::deleteUser(const HttpRequest& req) {
    try {
        string idStr = req.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("User ID is required")
            );
        }
        
        int userId = stoi(idStr);
        User* user = library->findUserByID(userId);
        
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))
            );
        }
        
        // Check if user has borrowed books
        if (user->getBorrowedBooksCount() > 0) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse(
                    "Cannot delete user with borrowed books. Please return all books first."
                )
            );
        }
        
        // TODO: Implement removeUser in Library class
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                "{\"deletedUserID\":" + to_string(userId) + "}",
                "User delete not yet implemented (requires Library::removeUser)"
            )
        );
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid user ID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to delete user: " + string(e.what()))
        );
    }
}

// GET /users/:id/borrowed - Get user's borrowed books
HttpResponse UserController::getBorrowedBooks(const HttpRequest& req) {
    try {
        string idStr = req.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("User ID is required")
            );
        }
        
        int userId = stoi(idStr);
        User* user = library->findUserByID(userId);
        
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))
            );
        }
        
        const vector<int> borrowedBookIDs = user->getBorrowedBookIDs();
        
        // Get full book details for each borrowed book
        stringstream ss;
        ss << "[";
        size_t count = 0;
        for (int bookId : borrowedBookIDs) {
            Book* book = library->findBookByID(bookId);
            if (book) {
                if (count > 0) ss << ",";
                
                ss << "{";
                ss << "\"bookID\":" << book->getBookID() << ",";
                ss << "\"title\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";
                ss << "\"author\":\"" << JsonHelper::escapeJson(book->getAuthor()) << "\",";
                ss << "\"category\":\"" << JsonHelper::escapeJson(book->getCategory()) << "\"";
                ss << "}";
                
                count++;
            }
        }
        ss << "]";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                ss.str(),
                "Retrieved " + to_string(count) + " borrowed books"
            )
        );
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid user ID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve borrowed books: " + string(e.what()))
        );
    }
}

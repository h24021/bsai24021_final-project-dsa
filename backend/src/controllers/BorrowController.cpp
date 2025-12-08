#include "../../include/controllers/BorrowController.h"
#include "../../include/http/HttpModels.h"
#include <sstream>

BorrowController::BorrowController(Library* lib) : library(lib) {}

HttpResponse BorrowController::borrowBook(const HttpRequest& req) {
    try {
        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());
        
        if (!body.count("userID") || !body.count("bookID")) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("userID and bookID are required")
            );
        }
        
        int userId = stoi(body["userID"]);
        int bookId = stoi(body["bookID"]);
        
        User* user = library->findUserByID(userId);
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found")
            );
        }
        
        Book* book = library->findBookByID(bookId);
        if (!book) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("Book not found")
            );
        }
        
        if (book->getAvailableCopies() <= 0) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("Book not available")
            );
        }
        
        const vector<int> borrowedBooks = user->getBorrowedBookIDs();
        for (int id : borrowedBooks) {
            if (id == bookId) {
                return HttpResponse::badRequest(
                    JsonHelper::createErrorResponse("User already borrowed this book")
                );
            }
        }
        
        bool success = library->borrowBook(userId, bookId);
        if (!success) {
            return HttpResponse::serverError(
                JsonHelper::createErrorResponse("Failed to borrow book")
            );
        }
        
        stringstream ss;
        ss << "{";
        ss << "\"message\":\"Book borrowed successfully\",";
        ss << "\"userID\":" << userId << ",";
        ss << "\"userName\":\"" << JsonHelper::escapeJson(user->getName()) << "\",";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";
        ss << "\"availableCopies\":" << book->getAvailableCopies();
        ss << "}";
        
        return HttpResponse::ok(ss.str());
        
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse(string("Error: ") + e.what())
        );
    }
}

HttpResponse BorrowController::returnBook(const HttpRequest& req) {
    try {
        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());
        
        if (!body.count("userID") || !body.count("bookID")) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("userID and bookID are required")
            );
        }
        
        int userId = stoi(body["userID"]);
        int bookId = stoi(body["bookID"]);
        
        User* user = library->findUserByID(userId);
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found")
            );
        }
        
        Book* book = library->findBookByID(bookId);
        if (!book) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("Book not found")
            );
        }
        
        const vector<int> borrowedBooks = user->getBorrowedBookIDs();
        bool hasBorrowed = false;
        for (int id : borrowedBooks) {
            if (id == bookId) {
                hasBorrowed = true;
                break;
            }
        }
        
        if (!hasBorrowed) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("User has not borrowed this book")
            );
        }
        
        bool success = library->returnBook(userId, bookId);
        if (!success) {
            return HttpResponse::serverError(
                JsonHelper::createErrorResponse("Failed to return book")
            );
        }
        
        stringstream ss;
        ss << "{";
        ss << "\"message\":\"Book returned successfully\",";
        ss << "\"userID\":" << userId << ",";
        ss << "\"userName\":\"" << JsonHelper::escapeJson(user->getName()) << "\",";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";
        ss << "\"availableCopies\":" << book->getAvailableCopies();
        ss << "}";
        
        return HttpResponse::ok(ss.str());
        
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse(string("Error: ") + e.what())
        );
    }
}

HttpResponse BorrowController::getBorrowHistory(const HttpRequest& req) {
    try {
        if (!req.hasPathParam("id")) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("Book ID required")
            );
        }
        
        int bookId = stoi(req.getPathParam("id"));
        
        Book* book = library->findBookByID(bookId);
        if (!book) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("Book not found")
            );
        }
        
        stringstream ss;
        ss << "{";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"title\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";
        ss << "\"availableCopies\":" << book->getAvailableCopies() << ",";
        ss << "\"history\":[]";
        ss << "}";
        
        return HttpResponse::ok(ss.str());
        
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse(string("Error: ") + e.what())
        );
    }
}

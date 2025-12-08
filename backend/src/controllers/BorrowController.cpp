#include "../../include/controllers/B        // Check if book is available
        if (book->getAvailableCopies() <= 0) {
           // Check if user has borrowed this book
        const vector<int> borrowedBook        // TODO: Implement actual borrow history tracking
        // For now, return basic info
        stringstream ss;
        ss << "{";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"title\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";
        ss << "\"availableCopies\":" << book->getAvailableCopies() << ",";
        ss << "\"currentlyBorrowed\":" << (book->getAvailableCopies() < book->getCopies() ? "true" : "false") << ",";
        ss << "\"history\":[]"; // TODO: Add actual history
        ss << "}";getBorrowedBookIDs();
        bool hasBorrowed = false;
        for (int id : borrowedBooks) {
            if (id == bookId) {
                hasBorrowed = true;
                break;
            }
        }return HttpResponse::badRequest(
                JsonHelper::createErrorResponse(
                    "Book is not available. Title: " + book->getTitle()
                )
            );
        }troller.h"
#include "../../include/http/HttpModels.h"
#include <sstream>

BorrowController::BorrowController(Library* lib) : library(lib) {}

// POST /borrow - Borrow a book
HttpResponse BorrowController::borrowBook(const HttpRequest& req) {
    try {
        // Parse JSON body
        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());
        
        if (!body.count("userID") || !body.count("bookID")) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("userID and bookID are required")
            );
        }
        
        int userId = stoi(body["userID"]);
        int bookId = stoi(body["bookID"]);
        
        // Check if user exists
        User* user = library->findUserByID(userId);
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))
            );
        }
        
        // Check if book exists
        Book* book = library->findBookByID(bookId);
        if (!book) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))
            );
        }
        
        // Check if book is available
        if (!bookOpt->isAvailable()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse(
                    "Book is not available. Title: " + bookOpt->getTitle()
                )
            );
        }
        
        // Attempt to borrow the book
        bool success = library->borrowBook(userId, bookId);
        
        if (!success) {
            return HttpResponse::serverError(
                JsonHelper::createErrorResponse("Failed to borrow book. Please try again.")
            );
        }
        
        // Get updated book and user info
        bookOpt = library->findBookByID(bookId);
        userOpt = library->findUserByID(userId);
        
        // Create response JSON
        stringstream ss;
        ss << "{";
        ss << "\"userID\":" << userId << ",";
        ss << "\"userName\":\"" << JsonHelper::escapeJson(userOpt->getName()) << "\",";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(bookOpt->getTitle()) << "\",";
        ss << "\"borrowedAt\":\"" << "now" << "\""; // TODO: Add timestamp
        ss << "}";
        
        return HttpResponse::created(
            JsonHelper::createSuccessResponse(
                ss.str(),
                "Book borrowed successfully"
            )
        );
        
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid userID or bookID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to borrow book: " + string(e.what()))
        );
    }
}

// POST /return - Return a book
HttpResponse BorrowController::returnBook(const HttpRequest& req) {
    try {
        // Parse JSON body
        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());
        
        if (!body.count("userID") || !body.count("bookID")) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("userID and bookID are required")
            );
        }
        
        int userId = stoi(body["userID"]);
        int bookId = stoi(body["bookID"]);
        
        // Check if user exists
        User* user = library->findUserByID(userId);
        if (!user) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))
            );
        }
        
        // Check if book exists
        Book* book = library->findBookByID(bookId);
        if (!book) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))
            );
        }
        
        // Check if user has borrowed this book
        const auto& borrowedBooks = userOpt->getBorrowedBookIDs();
        bool hasBorrowed = false;
        for (int id : borrowedBooks) {
            if (id == bookId) {
                hasBorrowed = true;
                break;
            }
        }
        
        if (!hasBorrowed) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse(
                    "User has not borrowed this book. Cannot return."
                )
            );
        }
        
        // Attempt to return the book
        bool success = library->returnBook(userId, bookId);
        
        if (!success) {
            return HttpResponse::serverError(
                JsonHelper::createErrorResponse("Failed to return book. Please try again.")
            );
        }
        
        // Get updated info
        bookOpt = library->findBookByID(bookId);
        userOpt = library->findUserByID(userId);
        
        // Create response JSON
        stringstream ss;
        ss << "{";
        ss << "\"userID\":" << userId << ",";
        ss << "\"userName\":\"" << JsonHelper::escapeJson(userOpt->getName()) << "\",";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(bookOpt->getTitle()) << "\",";
        ss << "\"returnedAt\":\"" << "now" << "\""; // TODO: Add timestamp
        ss << "}";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                ss.str(),
                "Book returned successfully"
            )
        );
        
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid userID or bookID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to return book: " + string(e.what()))
        );
    }
}

// GET /books/:id/history - Get borrow history for a book
HttpResponse BorrowController::getBorrowHistory(const HttpRequest& req) {
    try {
        string idStr = req.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest(
                JsonHelper::createErrorResponse("Book ID is required")
            );
        }
        
        int bookId = stoi(idStr);
        Book* book = library->findBookByID(bookId);
        
        if (!book) {
            return HttpResponse::notFound(
                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))
            );
        }
        
        // TODO: Implement actual borrow history tracking
        // For now, return basic info
        stringstream ss;
        ss << "{";
        ss << "\"bookID\":" << bookId << ",";
        ss << "\"title\":\"" << JsonHelper::escapeJson(bookOpt->getTitle()) << "\",";
        ss << "\"timesCirculated\":" << bookOpt->getTimesCirculated() << ",";
        ss << "\"currentlyBorrowed\":" << (bookOpt->isAvailable() ? "false" : "true") << ",";
        ss << "\"history\":[]"; // TODO: Add actual history
        ss << "}";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(ss.str())
        );
        
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid book ID format")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve borrow history: " + string(e.what()))
        );
    }
}

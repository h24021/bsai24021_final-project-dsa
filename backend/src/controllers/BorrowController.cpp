#include "../../include/controllers/BorrowController.h"#include "../../include/controllers/B        // Check if book is available

#include "../../include/http/HttpModels.h"        if (book->getAvailableCopies() <= 0) {

#include <sstream>           // Check if user has borrowed this book

        const vector<int> borrowedBook        // TODO: Implement actual borrow history tracking

BorrowController::BorrowController(Library* lib) : library(lib) {}        // For now, return basic info

        stringstream ss;

// POST /borrow - Borrow a book        ss << "{";

HttpResponse BorrowController::borrowBook(const HttpRequest& req) {        ss << "\"bookID\":" << bookId << ",";

    try {        ss << "\"title\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";

        // Parse JSON body        ss << "\"availableCopies\":" << book->getAvailableCopies() << ",";

        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());        ss << "\"currentlyBorrowed\":" << (book->getAvailableCopies() < book->getCopies() ? "true" : "false") << ",";

                ss << "\"history\":[]"; // TODO: Add actual history

        if (!body.count("userID") || !body.count("bookID")) {        ss << "}";getBorrowedBookIDs();

            return HttpResponse::badRequest(        bool hasBorrowed = false;

                JsonHelper::createErrorResponse("userID and bookID are required")        for (int id : borrowedBooks) {

            );            if (id == bookId) {

        }                hasBorrowed = true;

                        break;

        int userId = stoi(body["userID"]);            }

        int bookId = stoi(body["bookID"]);        }return HttpResponse::badRequest(

                        JsonHelper::createErrorResponse(

        // Check if user exists                    "Book is not available. Title: " + book->getTitle()

        User* user = library->findUserByID(userId);                )

        if (!user) {            );

            return HttpResponse::notFound(        }troller.h"

                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))#include "../../include/http/HttpModels.h"

            );#include <sstream>

        }

        BorrowController::BorrowController(Library* lib) : library(lib) {}

        // Check if book exists

        Book* book = library->findBookByID(bookId);// POST /borrow - Borrow a book

        if (!book) {HttpResponse BorrowController::borrowBook(const HttpRequest& req) {

            return HttpResponse::notFound(    try {

                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))        // Parse JSON body

            );        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());

        }        

                if (!body.count("userID") || !body.count("bookID")) {

        // Check if book is available            return HttpResponse::badRequest(

        if (book->getAvailableCopies() <= 0) {                JsonHelper::createErrorResponse("userID and bookID are required")

            return HttpResponse::badRequest(            );

                JsonHelper::createErrorResponse(        }

                    "Book is not available. Title: " + book->getTitle()        

                )        int userId = stoi(body["userID"]);

            );        int bookId = stoi(body["bookID"]);

        }        

                // Check if user exists

        // Attempt to borrow the book        User* user = library->findUserByID(userId);

        bool success = library->borrowBook(userId, bookId);        if (!user) {

                    return HttpResponse::notFound(

        if (!success) {                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))

            return HttpResponse::serverError(            );

                JsonHelper::createErrorResponse("Failed to borrow book. Please try again.")        }

            );        

        }        // Check if book exists

                Book* book = library->findBookByID(bookId);

        // Get updated book and user info        if (!book) {

        book = library->findBookByID(bookId);            return HttpResponse::notFound(

        user = library->findUserByID(userId);                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))

                    );

        // Create response JSON        }

        stringstream ss;        

        ss << "{";        // Check if book is available

        ss << "\"userID\":" << userId << ",";        if (!bookOpt->isAvailable()) {

        ss << "\"userName\":\"" << JsonHelper::escapeJson(user->getName()) << "\",";            return HttpResponse::badRequest(

        ss << "\"bookID\":" << bookId << ",";                JsonHelper::createErrorResponse(

        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";                    "Book is not available. Title: " + bookOpt->getTitle()

        ss << "\"borrowedAt\":\"" << "now" << "\""; // TODO: Add timestamp                )

        ss << "}";            );

                }

        return HttpResponse::created(        

            JsonHelper::createSuccessResponse(        // Attempt to borrow the book

                ss.str(),        bool success = library->borrowBook(userId, bookId);

                "Book borrowed successfully"        

            )        if (!success) {

        );            return HttpResponse::serverError(

                        JsonHelper::createErrorResponse("Failed to borrow book. Please try again.")

    } catch (const invalid_argument& e) {            );

        return HttpResponse::badRequest(        }

            JsonHelper::createErrorResponse("Invalid userID or bookID format")        

        );        // Get updated book and user info

    } catch (const exception& e) {        bookOpt = library->findBookByID(bookId);

        return HttpResponse::serverError(        userOpt = library->findUserByID(userId);

            JsonHelper::createErrorResponse("Failed to borrow book: " + string(e.what()))        

        );        // Create response JSON

    }        stringstream ss;

}        ss << "{";

        ss << "\"userID\":" << userId << ",";

// POST /return - Return a book        ss << "\"userName\":\"" << JsonHelper::escapeJson(userOpt->getName()) << "\",";

HttpResponse BorrowController::returnBook(const HttpRequest& req) {        ss << "\"bookID\":" << bookId << ",";

    try {        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(bookOpt->getTitle()) << "\",";

        // Parse JSON body        ss << "\"borrowedAt\":\"" << "now" << "\""; // TODO: Add timestamp

        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());        ss << "}";

                

        if (!body.count("userID") || !body.count("bookID")) {        return HttpResponse::created(

            return HttpResponse::badRequest(            JsonHelper::createSuccessResponse(

                JsonHelper::createErrorResponse("userID and bookID are required")                ss.str(),

            );                "Book borrowed successfully"

        }            )

                );

        int userId = stoi(body["userID"]);        

        int bookId = stoi(body["bookID"]);    } catch (const invalid_argument& e) {

                return HttpResponse::badRequest(

        // Check if user exists            JsonHelper::createErrorResponse("Invalid userID or bookID format")

        User* user = library->findUserByID(userId);        );

        if (!user) {    } catch (const exception& e) {

            return HttpResponse::notFound(        return HttpResponse::serverError(

                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))            JsonHelper::createErrorResponse("Failed to borrow book: " + string(e.what()))

            );        );

        }    }

        }

        // Check if book exists

        Book* book = library->findBookByID(bookId);// POST /return - Return a book

        if (!book) {HttpResponse BorrowController::returnBook(const HttpRequest& req) {

            return HttpResponse::notFound(    try {

                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))        // Parse JSON body

            );        map<string, string> body = JsonHelper::parseSimpleJson(req.getBody());

        }        

                if (!body.count("userID") || !body.count("bookID")) {

        // Check if user has borrowed this book            return HttpResponse::badRequest(

        const vector<int> borrowedBooks = user->getBorrowedBookIDs();                JsonHelper::createErrorResponse("userID and bookID are required")

        bool hasBorrowed = false;            );

        for (int id : borrowedBooks) {        }

            if (id == bookId) {        

                hasBorrowed = true;        int userId = stoi(body["userID"]);

                break;        int bookId = stoi(body["bookID"]);

            }        

        }        // Check if user exists

                User* user = library->findUserByID(userId);

        if (!hasBorrowed) {        if (!user) {

            return HttpResponse::badRequest(            return HttpResponse::notFound(

                JsonHelper::createErrorResponse(                JsonHelper::createErrorResponse("User not found with ID: " + to_string(userId))

                    "User has not borrowed this book. Cannot return."            );

                )        }

            );        

        }        // Check if book exists

                Book* book = library->findBookByID(bookId);

        // Attempt to return the book        if (!book) {

        bool success = library->returnBook(userId, bookId);            return HttpResponse::notFound(

                        JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))

        if (!success) {            );

            return HttpResponse::serverError(        }

                JsonHelper::createErrorResponse("Failed to return book. Please try again.")        

            );        // Check if user has borrowed this book

        }        const auto& borrowedBooks = userOpt->getBorrowedBookIDs();

                bool hasBorrowed = false;

        // Get updated info        for (int id : borrowedBooks) {

        book = library->findBookByID(bookId);            if (id == bookId) {

        user = library->findUserByID(userId);                hasBorrowed = true;

                        break;

        // Create response JSON            }

        stringstream ss;        }

        ss << "{";        

        ss << "\"userID\":" << userId << ",";        if (!hasBorrowed) {

        ss << "\"userName\":\"" << JsonHelper::escapeJson(user->getName()) << "\",";            return HttpResponse::badRequest(

        ss << "\"bookID\":" << bookId << ",";                JsonHelper::createErrorResponse(

        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";                    "User has not borrowed this book. Cannot return."

        ss << "\"returnedAt\":\"" << "now" << "\""; // TODO: Add timestamp                )

        ss << "}";            );

                }

        return HttpResponse::ok(        

            JsonHelper::createSuccessResponse(        // Attempt to return the book

                ss.str(),        bool success = library->returnBook(userId, bookId);

                "Book returned successfully"        

            )        if (!success) {

        );            return HttpResponse::serverError(

                        JsonHelper::createErrorResponse("Failed to return book. Please try again.")

    } catch (const invalid_argument& e) {            );

        return HttpResponse::badRequest(        }

            JsonHelper::createErrorResponse("Invalid userID or bookID format")        

        );        // Get updated info

    } catch (const exception& e) {        bookOpt = library->findBookByID(bookId);

        return HttpResponse::serverError(        userOpt = library->findUserByID(userId);

            JsonHelper::createErrorResponse("Failed to return book: " + string(e.what()))        

        );        // Create response JSON

    }        stringstream ss;

}        ss << "{";

        ss << "\"userID\":" << userId << ",";

// GET /books/:id/history - Get borrow history for a book        ss << "\"userName\":\"" << JsonHelper::escapeJson(userOpt->getName()) << "\",";

HttpResponse BorrowController::getBorrowHistory(const HttpRequest& req) {        ss << "\"bookID\":" << bookId << ",";

    try {        ss << "\"bookTitle\":\"" << JsonHelper::escapeJson(bookOpt->getTitle()) << "\",";

        string idStr = req.getPathParam("id");        ss << "\"returnedAt\":\"" << "now" << "\""; // TODO: Add timestamp

        if (idStr.empty()) {        ss << "}";

            return HttpResponse::badRequest(        

                JsonHelper::createErrorResponse("Book ID is required")        return HttpResponse::ok(

            );            JsonHelper::createSuccessResponse(

        }                ss.str(),

                        "Book returned successfully"

        int bookId = stoi(idStr);            )

        Book* book = library->findBookByID(bookId);        );

                

        if (!book) {    } catch (const invalid_argument& e) {

            return HttpResponse::notFound(        return HttpResponse::badRequest(

                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))            JsonHelper::createErrorResponse("Invalid userID or bookID format")

            );        );

        }    } catch (const exception& e) {

                return HttpResponse::serverError(

        // TODO: Implement actual borrow history tracking            JsonHelper::createErrorResponse("Failed to return book: " + string(e.what()))

        // For now, return basic info        );

        stringstream ss;    }

        ss << "{";}

        ss << "\"bookID\":" << bookId << ",";

        ss << "\"title\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";// GET /books/:id/history - Get borrow history for a book

        ss << "\"availableCopies\":" << book->getAvailableCopies() << ",";HttpResponse BorrowController::getBorrowHistory(const HttpRequest& req) {

        ss << "\"totalCopies\":" << book->getCopies() << ",";    try {

        ss << "\"currentlyBorrowed\":" << (book->getAvailableCopies() < book->getCopies() ? "true" : "false") << ",";        string idStr = req.getPathParam("id");

        ss << "\"history\":[]"; // TODO: Add actual history        if (idStr.empty()) {

        ss << "}";            return HttpResponse::badRequest(

                        JsonHelper::createErrorResponse("Book ID is required")

        return HttpResponse::ok(            );

            JsonHelper::createSuccessResponse(ss.str())        }

        );        

                int bookId = stoi(idStr);

    } catch (const invalid_argument& e) {        Book* book = library->findBookByID(bookId);

        return HttpResponse::badRequest(        

            JsonHelper::createErrorResponse("Invalid book ID format")        if (!book) {

        );            return HttpResponse::notFound(

    } catch (const exception& e) {                JsonHelper::createErrorResponse("Book not found with ID: " + to_string(bookId))

        return HttpResponse::serverError(            );

            JsonHelper::createErrorResponse("Failed to retrieve borrow history: " + string(e.what()))        }

        );        

    }        // TODO: Implement actual borrow history tracking

}        // For now, return basic info

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

#include "../../include/controllers/BookController.h"
#include <sstream>
#include <iostream>

BookController::BookController(Library* lib) : library(lib) {}

// Helper: Convert Book to JSON
string BookController::bookToJson(const Book& book) {
    map<string, string> fields;
    fields["id"] = to_string(book.getBookID());
    fields["title"] = book.getTitle();
    fields["author"] = book.getAuthor();
    fields["isbn"] = book.getISBN();
    fields["category"] = book.getCategory();
    fields["copies"] = to_string(book.getCopies());
    fields["availableCopies"] = to_string(book.getAvailableCopies());
    
    return JsonHelper::createObject(fields);
}

// Helper: Convert vector of Books to JSON array
string BookController::booksToJson(const vector<Book>& books) {
    vector<string> bookJsons;
    for (const auto& book : books) {
        bookJsons.push_back(bookToJson(book));
    }
    return JsonHelper::createArray(bookJsons);
}

// GET /books - Get all books
HttpResponse BookController::getAllBooks(const HttpRequest& request) {
    try {
        auto books = library->getAllBooks();
        
        map<string, string> response;
        response["status"] = "success";
        response["data"] = booksToJson(books);
        response["count"] = to_string(books.size());
        
        string json = JsonHelper::createObject(response);
        return HttpResponse::ok(json);
        
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

// GET /books/:id - Get book by ID
HttpResponse BookController::getBookById(const HttpRequest& request) {
    try {
        string idStr = request.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest("Book ID is required");
        }
        
        int id = stoi(idStr);
        Book* book = library->findBookByID(id);
        
        if (book == nullptr) {
            return HttpResponse::notFound("Book not found with ID: " + idStr);
        }
        
        string json = JsonHelper::createSuccessResponse(bookToJson(*book));
        return HttpResponse::ok(json);
        
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest("Invalid book ID format");
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

// GET /books/search?title=...&author=...&category=... - Search books
HttpResponse BookController::searchBooks(const HttpRequest& request) {
    try {
        vector<Book> results;
        
        // Search by title
        if (request.hasQueryParam("title")) {
            string title = request.getQueryParam("title");
            results = library->searchBookByTitle(title);
        }
        // Search by author
        else if (request.hasQueryParam("author")) {
            string author = request.getQueryParam("author");
            results = library->searchBookByAuthor(author);
        }
        // Search by category
        else if (request.hasQueryParam("category")) {
            string category = request.getQueryParam("category");
            results = library->searchBookByCategory(category);
        }
        else {
            return HttpResponse::badRequest("Please provide title, author, or category parameter");
        }
        
        map<string, string> response;
        response["status"] = "success";
        response["data"] = booksToJson(results);
        response["count"] = to_string(results.size());
        
        string json = JsonHelper::createObject(response);
        return HttpResponse::ok(json);
        
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

// POST /books - Create new book
HttpResponse BookController::createBook(const HttpRequest& request) {
    try {
        // Parse JSON body
        auto fields = JsonHelper::parseSimpleJson(request.getBody());
        
        if (fields.find("title") == fields.end() || 
            fields.find("author") == fields.end()) {
            return HttpResponse::badRequest("Title and author are required");
        }
        
        // Generate new ID (in production, this would be auto-incremented)
        static int nextId = 1000;
        int id = nextId++;
        
        string title = fields["title"];
        string author = fields["author"];
        string isbn = (fields.find("isbn") != fields.end()) ? fields["isbn"] : "";
        string category = (fields.find("category") != fields.end()) ? fields["category"] : "General";
        int copies = (fields.find("copies") != fields.end()) ? stoi(fields["copies"]) : 1;
        
        Book book(id, title, author, isbn, category, copies, copies);
        library->addBook(book);
        
        string json = JsonHelper::createSuccessResponse(bookToJson(book), "Book added successfully");
        return HttpResponse::created(json);
        
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

// PUT /books/:id - Update book
HttpResponse BookController::updateBook(const HttpRequest& request) {
    try {
        string idStr = request.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest("Book ID is required");
        }
        
        int id = stoi(idStr);
        Book* existingBook = library->findBookByID(id);
        
        if (existingBook == nullptr) {
            return HttpResponse::notFound("Book not found with ID: " + idStr);
        }
        
        // Parse update fields
        auto fields = JsonHelper::parseSimpleJson(request.getBody());
        
        // For now, we'll just return success
        // In a full implementation, you'd update the book in the B-Tree
        
        string json = JsonHelper::createSuccessResponse(
            bookToJson(*existingBook), 
            "Book updated successfully"
        );
        return HttpResponse::ok(json);
        
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

// DELETE /books/:id - Delete book
HttpResponse BookController::deleteBook(const HttpRequest& request) {
    try {
        string idStr = request.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest("Book ID is required");
        }
        
        int id = stoi(idStr);
        Book* book = library->findBookByID(id);
        
        if (book == nullptr) {
            return HttpResponse::notFound("Book not found with ID: " + idStr);
        }
        
        // For now, just return success
        // In full implementation, you'd remove from B-Tree
        
        map<string, string> response;
        response["status"] = "success";
        response["message"] = "Book deleted successfully";
        
        string json = JsonHelper::createObject(response);
        return HttpResponse::ok(json);
        
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

// GET /books/:id/history - Get borrow history
HttpResponse BookController::getBorrowHistory(const HttpRequest& request) {
    try {
        string idStr = request.getPathParam("id");
        if (idStr.empty()) {
            return HttpResponse::badRequest("Book ID is required");
        }
        
        int id = stoi(idStr);
        Book* book = library->findBookByID(id);
        
        if (book == nullptr) {
            return HttpResponse::notFound("Book not found with ID: " + idStr);
        }
        
        // For now, return empty history
        // In full implementation, track borrow history
        
        map<string, string> response;
        response["status"] = "success";
        response["data"] = "[]";
        response["count"] = "0";
        
        string json = JsonHelper::createObject(response);
        return HttpResponse::ok(json);
        
    } catch (const exception& e) {
        return HttpResponse::serverError(e.what());
    }
}

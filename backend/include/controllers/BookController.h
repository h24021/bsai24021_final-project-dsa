#pragma once
#include <string>
#include <vector>
#include "../models/Book.h"
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

/**
 * BookController - Handles REST API requests for book operations
 */
class BookController {
private:
    Library* library;
    
    // JSON serialization helpers
    string bookToJson(const Book& book);
    string booksToJson(const vector<Book>& books);

public:
    BookController(Library* lib);

    // REST API endpoints
    HttpResponse getAllBooks(const HttpRequest& request);
    HttpResponse getBookById(const HttpRequest& request);
    HttpResponse searchBooks(const HttpRequest& request);
    HttpResponse createBook(const HttpRequest& request);
    HttpResponse updateBook(const HttpRequest& request);
    HttpResponse deleteBook(const HttpRequest& request);
    HttpResponse getBorrowHistory(const HttpRequest& request);
};


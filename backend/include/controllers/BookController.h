#pragma once
#include <string>
#include <vector>
#include "../models/Book.h"
#include "../services/Library.h"

using namespace std;

/**
 * BookController - Handles HTTP-like requests for book operations
 * This will be the bridge between API endpoints and business logic
 */
class BookController {
private:
    Library* library;

public:
    BookController(Library* lib);

    // REST-like methods that will be exposed via API
    string getAllBooks();           // GET /api/books
    string getBookById(int id);     // GET /api/books/:id
    string getBookByISBN(string isbn); // GET /api/books/isbn/:isbn
    string createBook(int id, string title, string author, string isbn, 
                     string category, int copies, int available); // POST /api/books
    string updateBook(int id, string title, string author, string isbn,
                     string category, int copies, int available); // PUT /api/books/:id
    string deleteBook(int id);      // DELETE /api/books/:id
    
    // JSON response helpers
    string toJSON(const Book& book);
    string errorResponse(string message, int code);
    string successResponse(string message);
};

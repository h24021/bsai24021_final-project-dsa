#include "../../include/controllers/BookController.h"
#include <sstream>

BookController::BookController(Library* lib) : library(lib) {}

string BookController::getAllBooks() {
    // TODO: Implement fetching all books and return as JSON array
    return "{\"status\":\"success\",\"data\":[]}";
}

string BookController::getBookById(int id) {
    // TODO: Implement search by ID and return JSON
    return "{\"status\":\"success\",\"data\":null}";
}

string BookController::getBookByISBN(string isbn) {
    // TODO: Implement search by ISBN and return JSON
    library->searchBookByISBN(isbn); // Current implementation prints
    return "{\"status\":\"success\",\"data\":null}";
}

string BookController::createBook(int id, string title, string author, 
                                  string isbn, string category, int copies, int available) {
    Book newBook(id, title, author, isbn, category, copies, available);
    library->addBook(newBook);
    return successResponse("Book created successfully");
}

string BookController::updateBook(int id, string title, string author,
                                  string isbn, string category, int copies, int available) {
    // TODO: Implement book update logic
    return successResponse("Book updated successfully");
}

string BookController::deleteBook(int id) {
    // TODO: Implement book deletion logic
    return successResponse("Book deleted successfully");
}

string BookController::toJSON(const Book& book) {
    stringstream ss;
    ss << "{"
       << "\"bookID\":" << book.getBookID() << ","
       << "\"title\":\"" << book.getTitle() << "\","
       << "\"isbn\":\"" << book.getISBN() << "\""
       << "}";
    return ss.str();
}

string BookController::errorResponse(string message, int code) {
    stringstream ss;
    ss << "{\"status\":\"error\",\"code\":" << code 
       << ",\"message\":\"" << message << "\"}";
    return ss.str();
}

string BookController::successResponse(string message) {
    return "{\"status\":\"success\",\"message\":\"" + message + "\"}";
}

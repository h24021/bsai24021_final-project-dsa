#include <iostream>
#include <string>
#include "../include/services/Library.h"
#include "../include/api/Router.h"
#include "../include/controllers/BookController.h"
#include "../include/controllers/UserController.h"
#include "../include/controllers/BorrowController.h"
#include "../include/controllers/StatisticsController.h"
#include "../include/http/HttpServer.h"

using namespace std;

static void printBanner() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║     Library Management System - HTTP API Server         ║\n";
    cout << "║  (exposes Router over TCP with JSON + CORS)             ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n\n";
}

static void seedSampleData(Library& library) {
    library.addBook(Book(101, "The Great Gatsby", "F. Scott Fitzgerald", "ISBN-101", "Fiction", 3, 3));
    library.addBook(Book(102, "To Kill a Mockingbird", "Harper Lee", "ISBN-102", "Fiction", 3, 3));
    library.addBook(Book(103, "1984", "George Orwell", "ISBN-103", "Dystopian", 3, 3));
    library.addBook(Book(104, "Pride and Prejudice", "Jane Austen", "ISBN-104", "Romance", 3, 3));
    library.addBook(Book(105, "The Catcher in the Rye", "J.D. Salinger", "ISBN-105", "Fiction", 3, 3));
    library.addBook(Book(106, "Animal Farm", "George Orwell", "ISBN-106", "Political Fiction", 3, 3));
    library.addBook(Book(107, "Lord of the Flies", "William Golding", "ISBN-107", "Adventure", 3, 3));
    library.addBook(Book(108, "Brave New World", "Aldous Huxley", "ISBN-108", "Dystopian", 3, 3));
    library.addBook(Book(109, "The Hobbit", "J.R.R. Tolkien", "ISBN-109", "Fantasy", 3, 3));
    library.addBook(Book(110, "Fahrenheit 451", "Ray Bradbury", "ISBN-110", "Science Fiction", 3, 3));

    library.addUser(User(1001, "Alice Johnson", "alice@library.com", "librarian"));
    library.addUser(User(1002, "Bob Smith", "bob@library.com", "member"));
    library.addUser(User(1003, "Carol Davis", "carol@library.com", "member"));
}

static void registerRoutes(Router& router, Library& library,
                           BookController& bookController,
                           UserController& userController,
                           BorrowController& borrowController,
                           StatisticsController& statsController) {
    // Book routes
    router.get("/books", [&](const HttpRequest& req) { return bookController.getAllBooks(req); });
    router.get("/books/:id", [&](const HttpRequest& req) { return bookController.getBookById(req); });
    router.get("/books/search", [&](const HttpRequest& req) { return bookController.searchBooks(req); });
    router.post("/books", [&](const HttpRequest& req) { return bookController.createBook(req); });
    router.put("/books/:id", [&](const HttpRequest& req) { return bookController.updateBook(req); });
    router.del("/books/:id", [&](const HttpRequest& req) { return bookController.deleteBook(req); });

    // User routes
    router.get("/users", [&](const HttpRequest& req) { return userController.getAllUsers(req); });
    router.get("/users/:id", [&](const HttpRequest& req) { return userController.getUserById(req); });
    router.get("/users/email/:email", [&](const HttpRequest& req) { return userController.getUserByEmail(req); });
    router.get("/users/:id/borrowed", [&](const HttpRequest& req) { return userController.getBorrowedBooks(req); });
    router.post("/users", [&](const HttpRequest& req) { return userController.createUser(req); });
    router.put("/users/:id", [&](const HttpRequest& req) { return userController.updateUser(req); });
    router.del("/users/:id", [&](const HttpRequest& req) { return userController.deleteUser(req); });

    // Borrow routes
    router.post("/borrow", [&](const HttpRequest& req) { return borrowController.borrowBook(req); });
    router.post("/return", [&](const HttpRequest& req) { return borrowController.returnBook(req); });
    router.get("/books/:id/history", [&](const HttpRequest& req) { return borrowController.getBorrowHistory(req); });

    // Stats routes
    router.get("/dashboard", [&](const HttpRequest& req) { return statsController.getDashboard(req); });
    router.get("/statistics/most-borrowed", [&](const HttpRequest& req) { return statsController.getMostBorrowedBooks(req); });
    router.get("/statistics/most-active",  [&](const HttpRequest& req) { return statsController.getMostActiveUsers(req); });
    router.get("/statistics/category-distribution", [&](const HttpRequest& req) { return statsController.getCategoryDistribution(req); });
}

int main() {
    printBanner();
    Library library;
    seedSampleData(library);

    Router router("/api/v1");
    
    // Create controllers that will live for the entire program
    BookController bookController(&library);
    UserController userController(&library);
    BorrowController borrowController(&library);
    StatisticsController statsController(&library);
    
    registerRoutes(router, library, bookController, userController, borrowController, statsController);
    cout << "Registered routes: " << router.getRouteCount() << " under base path " << router.getBasePath() << "\n";

    HttpServer server(router, 8080);
    cout << "Starting HTTP server...\n";
    server.start();
    return 0;
}

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../include/services/Library.h"
#include "../include/services/SessionManager.h"
#include "../include/api/Router.h"
#include "../include/controllers/BookController.h"
#include "../include/controllers/UserController.h"
#include "../include/controllers/BorrowController.h"
#include "../include/controllers/StatisticsController.h"
#include "../include/controllers/AuthController.h"
#include "../include/http/HttpServer.h"
#include "../include/utils/DataLoader.h"

using namespace std;

static void printBanner() {
    cout << "\n";
    cout << "===========================================================\n";
    cout << "     Library Management System - HTTP API Server         \n";
    cout << "  (exposes Router over TCP with JSON + CORS)             \n";
    cout << "===========================================================\n\n";
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

    
}

static HttpResponse serveApiDocs(const HttpRequest& req) {
    std::ifstream file("backend/api-docs.html");
    if (!file.is_open()) {
        return HttpResponse::notFound("API documentation file not found");
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    HttpResponse response(HttpStatus::OK);
    response.setBody(buffer.str());
    response.setHeader("Content-Type", "text/html");
    return response;
}

static void registerRoutes(Router& router, Library& library,
                           BookController& bookController,
                           UserController& userController,
                           BorrowController& borrowController,
                           StatisticsController& statsController,
                           AuthController& authController) {
    router.get("/docs", serveApiDocs);
    
    // Authentication routes
    router.post("/auth/login", [&](const HttpRequest& req) { return authController.login(req); });
    router.post("/auth/register", [&](const HttpRequest& req) { return authController.registerUser(req); });
    router.post("/auth/logout", [&](const HttpRequest& req) { return authController.logout(req); });
    router.get("/auth/me", [&](const HttpRequest& req) { return authController.getCurrentUser(req); });
    
    router.get("/books", [&](const HttpRequest& req) { return bookController.getAllBooks(req); });
    router.get("/books/search", [&](const HttpRequest& req) { return bookController.searchBooks(req); }); 
    router.get("/books/:id", [&](const HttpRequest& req) { return bookController.getBookById(req); });
    router.post("/books", [&](const HttpRequest& req) { return bookController.createBook(req); });
    router.put("/books/:id", [&](const HttpRequest& req) { return bookController.updateBook(req); });
    router.del("/books/:id", [&](const HttpRequest& req) { return bookController.deleteBook(req); });


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


    router.get("/dashboard", [&](const HttpRequest& req) { return statsController.getDashboard(req); });
    router.get("/statistics/most-borrowed", [&](const HttpRequest& req) { return statsController.getMostBorrowedBooks(req); });
    router.get("/statistics/most-active",  [&](const HttpRequest& req) { return statsController.getMostActiveUsers(req); });
    router.get("/statistics/category-distribution", [&](const HttpRequest& req) { return statsController.getCategoryDistribution(req); });
}

int main() {
    printBanner();
    Library library;
    SessionManager sessionManager;
    
    if (!DataLoader::loadFromFile(library, "library_data.json")) {
        cout << "Could not load library_data.json, using sample data instead...\n";
        seedSampleData(library);
    }

    Router router("/api/v1");
    
    BookController bookController(&library);
    UserController userController(&library);
    BorrowController borrowController(&library);
    StatisticsController statsController(&library);
    AuthController authController(&library, &sessionManager);
    
    registerRoutes(router, library, bookController, userController, borrowController, statsController, authController);
    cout << "Registered routes: " << router.getRouteCount() << " under base path " << router.getBasePath() << "\n";

    HttpServer server(router, 8080);
    cout << "Starting HTTP server...\n";
    server.start();
    return 0;
}

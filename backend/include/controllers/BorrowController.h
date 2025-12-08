#pragma once
#include <string>
#include "../models/Book.h"
#include "../models/User.h"
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

/**
 * BorrowController - Handles REST API requests for borrow/return operations
 * Manages book borrowing and returning transactions
 */
class BorrowController {
private:
    Library* library;

public:
    BorrowController(Library* lib);

    // REST API endpoint handlers
    HttpResponse borrowBook(const HttpRequest& req);      // POST /borrow
    HttpResponse returnBook(const HttpRequest& req);      // POST /return
    HttpResponse getBorrowHistory(const HttpRequest& req); // GET /books/:id/history
};

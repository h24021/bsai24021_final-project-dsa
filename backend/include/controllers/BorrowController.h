#pragma once
#include <string>
#include "../models/Book.h"
#include "../models/User.h"
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

class BorrowController {
private:
    Library* library;

public:
    BorrowController(Library* lib);

    HttpResponse borrowBook(const HttpRequest& req);       
    HttpResponse returnBook(const HttpRequest& req);       
    HttpResponse getBorrowHistory(const HttpRequest& req);  
};

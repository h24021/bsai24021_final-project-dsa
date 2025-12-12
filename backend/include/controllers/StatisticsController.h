#pragma once
#include <string>
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

class StatisticsController {
private:
    Library* library;

public:
    StatisticsController(Library* lib);

    HttpResponse getDashboard(const HttpRequest& req);
    HttpResponse getMostBorrowedBooks(const HttpRequest& req);
    HttpResponse getMostActiveUsers(const HttpRequest& req);
    HttpResponse getCategoryDistribution(const HttpRequest& req);
};

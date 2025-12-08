#pragma once
#include <string>
#include "../services/Library.h"
#include "../http/HttpModels.h"

using namespace std;

/**
 * StatisticsController - Handles REST API requests for statistics and analytics
 * Provides dashboard data, reports, and insights about library usage
 */
class StatisticsController {
private:
    Library* library;

public:
    StatisticsController(Library* lib);

    // REST API endpoint handlers
    HttpResponse getDashboard(const HttpRequest& req);           // GET /dashboard
    HttpResponse getMostBorrowedBooks(const HttpRequest& req);   // GET /statistics/most-borrowed
    HttpResponse getMostActiveUsers(const HttpRequest& req);     // GET /statistics/most-active
    HttpResponse getCategoryDistribution(const HttpRequest& req); // GET /statistics/category-distribution
};

#include "../../include/controllers/StatisticsController.h"
#include "../../include/http/HttpModels.h"
#include <sstream>
#include <vector>
#include <map>

StatisticsController::StatisticsController(Library* lib) : library(lib) {}

HttpResponse StatisticsController::getDashboard(const HttpRequest& req) {
    (void)req;  
    
    try {
        vector<Book> allBooks = library->getAllBooks();
        vector<User> allUsers = library->getAllUsers();

        int totalBooks = (int)allBooks.size();
        int availableBooks = 0;
        int borrowedBooks = 0;
        
        for (const auto& book : allBooks) {
            if (book.getAvailableCopies() > 0) {
                availableBooks++;
            } else {
                borrowedBooks++;
            }
        }

        int totalBorrowedInstances = 0;
        for (const auto& user : allUsers) {
            totalBorrowedInstances += (int)user.getBorrowedBookIDs().size();
        }

        map<string, int> categoryCount;
        for (const auto& book : allBooks) {
            categoryCount[book.getCategory()]++;
        }

        stringstream categorySS;
        categorySS << "[";
        size_t catIdx = 0;
        for (const auto& entry : categoryCount) {
            categorySS << "{";
            categorySS << "\"category\":\"" << JsonHelper::escapeJson(entry.first) << "\",";
            categorySS << "\"count\":" << entry.second;
            categorySS << "}";
            if (catIdx < categoryCount.size() - 1) categorySS << ",";
            catIdx++;
        }
        categorySS << "]";

        stringstream ss;
        ss << "{";
        ss << "\"overview\":{";
        ss << "\"totalBooks\":" << totalBooks << ",";
        ss << "\"availableBooks\":" << availableBooks << ",";
        ss << "\"borrowedBooks\":" << borrowedBooks << ",";
        ss << "\"totalUsers\":" << allUsers.size() << ",";
        ss << "\"totalBorrowedInstances\":" << totalBorrowedInstances;
        ss << "},";
        ss << "\"categoryDistribution\":" << categorySS.str();
        ss << "}";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(ss.str(), "Dashboard data retrieved successfully")
        );
        
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve dashboard: " + string(e.what()))
        );
    }
}

HttpResponse StatisticsController::getMostBorrowedBooks(const HttpRequest& req) {
    try {
         
        string limitStr = req.getQueryParam("limit");
        int limit = limitStr.empty() ? 10 : stoi(limitStr);
        if (limit <= 0) limit = 10;
        if (limit > 100) limit = 100;  
        
        vector<pair<int, int>> mostBorrowed = library->getMostBorrowedBooks(limit);
        
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < mostBorrowed.size(); i++) {
            int bookID = mostBorrowed[i].first;
            int count = mostBorrowed[i].second;
            
            Book* book = library->findBookByID(bookID);
            if (book) {
                ss << "{";
                ss << "\"bookID\":" << bookID << ",";
                ss << "\"title\":\"" << JsonHelper::escapeJson(book->getTitle()) << "\",";
                ss << "\"author\":\"" << JsonHelper::escapeJson(book->getAuthor()) << "\",";
                ss << "\"category\":\"" << JsonHelper::escapeJson(book->getCategory()) << "\",";
                ss << "\"timesCirculated\":" << count;
                ss << "}";
                
                if (i < mostBorrowed.size() - 1) ss << ",";
            }
        }
        ss << "]";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                ss.str(),
                "Retrieved top " + to_string(mostBorrowed.size()) + " most borrowed books"
            )
        );
        
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid limit parameter")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve most borrowed books: " + string(e.what()))
        );
    }
}

HttpResponse StatisticsController::getMostActiveUsers(const HttpRequest& req) {
    try {
         
        string limitStr = req.getQueryParam("limit");
        int limit = limitStr.empty() ? 10 : stoi(limitStr);
        if (limit <= 0) limit = 10;
        if (limit > 100) limit = 100;  
        
        vector<pair<int, int>> mostActive = library->getMostActiveUsers(limit);
        
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < mostActive.size(); i++) {
            int userID = mostActive[i].first;
            int count = mostActive[i].second;
            
            User* user = library->findUserByID(userID);
            if (user) {
                ss << "{";
                ss << "\"userID\":" << userID << ",";
                ss << "\"name\":\"" << JsonHelper::escapeJson(user->getName()) << "\",";
                ss << "\"email\":\"" << JsonHelper::escapeJson(user->getEmail()) << "\",";
                ss << "\"role\":\"" << JsonHelper::escapeJson(user->getRole()) << "\",";
                ss << "\"booksBorrowed\":" << count << ",";
                ss << "\"currentlyBorrowed\":" << user->getBorrowedBookIDs().size();
                ss << "}";
                
                if (i < mostActive.size() - 1) ss << ",";
            }
        }
        ss << "]";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                ss.str(),
                "Retrieved top " + to_string(mostActive.size()) + " most active users"
            )
        );
        
    } catch (const invalid_argument& e) {
        return HttpResponse::badRequest(
            JsonHelper::createErrorResponse("Invalid limit parameter")
        );
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve most active users: " + string(e.what()))
        );
    }
}

HttpResponse StatisticsController::getCategoryDistribution(const HttpRequest& req) {
    (void)req;  
    
    try {
        vector<Book> allBooks = library->getAllBooks();

        map<string, int> categoryCount;
        map<string, int> availableCount;
        map<string, int> borrowedCount;
        
        for (const auto& book : allBooks) {
            string category = book.getCategory();
            categoryCount[category]++;
            
            if (book.getAvailableCopies() > 0) {
                availableCount[category]++;
            } else {
                borrowedCount[category]++;
            }
        }

        stringstream ss;
        ss << "[";
        size_t idx = 0;
        for (const auto& entry : categoryCount) {
            string category = entry.first;
            int total = entry.second;
            int available = availableCount[category];
            int borrowed = borrowedCount[category];
            
            ss << "{";
            ss << "\"category\":\"" << JsonHelper::escapeJson(category) << "\",";
            ss << "\"totalBooks\":" << total << ",";
            ss << "\"availableBooks\":" << available << ",";
            ss << "\"borrowedBooks\":" << borrowed << ",";
            ss << "\"borrowRate\":" << (total > 0 ? (borrowed * 100.0 / total) : 0);
            ss << "}";
            
            if (idx < categoryCount.size() - 1) ss << ",";
            idx++;
        }
        ss << "]";
        
        return HttpResponse::ok(
            JsonHelper::createSuccessResponse(
                ss.str(),
                "Retrieved category distribution for " + to_string(categoryCount.size()) + " categories"
            )
        );
        
    } catch (const exception& e) {
        return HttpResponse::serverError(
            JsonHelper::createErrorResponse("Failed to retrieve category distribution: " + string(e.what()))
        );
    }
}

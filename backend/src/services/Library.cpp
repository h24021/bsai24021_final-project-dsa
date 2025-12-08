#include "../../include/services/Library.h"
#include <iostream>
#include <iomanip>
using namespace std;

Library::Library() {
    // Initialize B-Tree with degree 3 and title comparison
    booksByTitle = new BTree<Book>(3, Book::compareByTitle);
}

Library::~Library() {
    delete booksByTitle;
}

// ============== Book Operations ==============

void Library::addBook(const Book& b) {
    booksByTitle->insert(b);
    cout << "Book added: " << b.getTitle() << " by " << b.getAuthor() << endl;
}

void Library::printAllBooks() {
    cout << "\n========== ALL BOOKS ==========\n";
    booksByTitle->traverse([](const Book& b) {
        b.printBook();
    });
    cout << "================================\n\n";
}

vector<Book> Library::searchBookByTitle(const string& title) {
    string searchLower = title;
    transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    
    return booksByTitle->searchByPredicate([searchLower](const Book& b) {
        string bookTitle = b.getTitle();
        transform(bookTitle.begin(), bookTitle.end(), bookTitle.begin(), ::tolower);
        return bookTitle.find(searchLower) != string::npos;
    });
}

vector<Book> Library::searchBookByAuthor(const string& author) {
    string searchLower = author;
    transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    
    return booksByTitle->searchByPredicate([searchLower](const Book& b) {
        string bookAuthor = b.getAuthor();
        transform(bookAuthor.begin(), bookAuthor.end(), bookAuthor.begin(), ::tolower);
        return bookAuthor.find(searchLower) != string::npos;
    });
}

vector<Book> Library::searchBookByCategory(const string& category) {
    string searchLower = category;
    transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    
    return booksByTitle->searchByPredicate([searchLower](const Book& b) {
        string bookCategory = b.getCategory();
        transform(bookCategory.begin(), bookCategory.end(), bookCategory.begin(), ::tolower);
        return bookCategory == searchLower;
    });
}

Book* Library::findBookByID(int bookID) {
    auto books = booksByTitle->searchByPredicate([bookID](const Book& b) {
        return b.getBookID() == bookID;
    });
    
    if (!books.empty()) {
        // Return pointer to first match (stored in static to persist)
        static Book foundBook;
        foundBook = books[0];
        return &foundBook;
    }
    return nullptr;
}

// ============== User Operations ==============

void Library::addUser(const User& u) {
    usersByID.insert(u.getUserID(), u);
    usersByEmail.insert(u.getEmail(), u);
    cout << "User added: " << u.getName() << " (ID: " << u.getUserID() << ")" << endl;
}

User* Library::findUserByID(int userID) {
    auto result = usersByID.find(userID);
    if (result.has_value()) {
        static User foundUser;
        foundUser = result.value();
        return &foundUser;
    }
    return nullptr;
}

User* Library::findUserByEmail(const string& email) {
    auto result = usersByEmail.find(email);
    if (result.has_value()) {
        static User foundUser;
        foundUser = result.value();
        return &foundUser;
    }
    return nullptr;
}

void Library::printAllUsers() {
    cout << "\n========== ALL USERS ==========\n";
    auto users = usersByID.getAllValues();
    for (const auto& u : users) {
        u.printUser();
    }
    cout << "================================\n\n";
}

// ============== Borrow/Return Operations ==============

bool Library::borrowBook(int userID, int bookID) {
    // Find user
    auto userOpt = usersByID.find(userID);
    if (!userOpt.has_value()) {
        cout << "Error: User ID " << userID << " not found.\n";
        return false;
    }
    
    User user = userOpt.value();
    
    // Check if user already borrowed this book
    if (user.hasBorrowedBook(bookID)) {
        cout << "Error: User has already borrowed this book.\n";
        return false;
    }
    
    // Find book
    auto books = booksByTitle->searchByPredicate([bookID](const Book& b) {
        return b.getBookID() == bookID;
    });
    
    if (books.empty()) {
        cout << "Error: Book ID " << bookID << " not found.\n";
        return false;
    }
    
    Book book = books[0];
    
    // Check availability
    if (book.getAvailableCopies() <= 0) {
        cout << "Error: No copies available for \"" << book.getTitle() << "\".\n";
        return false;
    }
    
    // Update user
    user.borrowBook(bookID);
    usersByID.insert(userID, user);
    usersByEmail.insert(user.getEmail(), user);
    
    // Update book availability (Note: This is a limitation - we need to update in B-Tree)
    // For now, we'll track this separately
    
    // Update borrow count statistics
    auto countOpt = borrowCounts.find(bookID);
    int count = countOpt.has_value() ? countOpt.value() : 0;
    borrowCounts.insert(bookID, count + 1);
    
    cout << "Success: \"" << book.getTitle() << "\" borrowed by " << user.getName() << endl;
    return true;
}

bool Library::returnBook(int userID, int bookID) {
    // Find user
    auto userOpt = usersByID.find(userID);
    if (!userOpt.has_value()) {
        cout << "Error: User ID " << userID << " not found.\n";
        return false;
    }
    
    User user = userOpt.value();
    
    // Check if user has borrowed this book
    if (!user.hasBorrowedBook(bookID)) {
        cout << "Error: User has not borrowed this book.\n";
        return false;
    }
    
    // Find book
    auto books = booksByTitle->searchByPredicate([bookID](const Book& b) {
        return b.getBookID() == bookID;
    });
    
    if (books.empty()) {
        cout << "Error: Book ID " << bookID << " not found.\n";
        return false;
    }
    
    Book book = books[0];
    
    // Update user
    user.returnBook(bookID);
    usersByID.insert(userID, user);
    usersByEmail.insert(user.getEmail(), user);
    
    cout << "Success: \"" << book.getTitle() << "\" returned by " << user.getName() << endl;
    return true;
}

// ============== Statistics ==============

vector<pair<int, int>> Library::getMostBorrowedBooks(int topN) {
    vector<pair<int, int>> bookCounts;
    auto bookIDs = borrowCounts.getAllKeys();
    
    for (int id : bookIDs) {
        auto count = borrowCounts.find(id);
        if (count.has_value()) {
            bookCounts.push_back({id, count.value()});
        }
    }
    
    // Sort by count descending
    sort(bookCounts.begin(), bookCounts.end(), 
         [](const pair<int,int>& a, const pair<int,int>& b) {
             return a.second > b.second;
         });
    
    // Return top N
    if ((int)bookCounts.size() > topN) {
        bookCounts.resize(topN);
    }
    
    return bookCounts;
}

vector<pair<int, int>> Library::getMostActiveUsers(int topN) {
    vector<pair<int, int>> userActivity;
    auto users = usersByID.getAllValues();
    
    for (const auto& user : users) {
        userActivity.push_back({user.getUserID(), user.getBorrowedBooksCount()});
    }
    
    // Sort by activity descending
    sort(userActivity.begin(), userActivity.end(),
         [](const pair<int,int>& a, const pair<int,int>& b) {
             return a.second > b.second;
         });
    
    // Return top N
    if ((int)userActivity.size() > topN) {
        userActivity.resize(topN);
    }
    
    return userActivity;
}

void Library::printStatistics() {
    cout << "\n========== LIBRARY STATISTICS ==========\n";
    cout << "Total Books: " << getTotalBooks() << endl;
    cout << "Total Users: " << getTotalUsers() << endl;
    
    cout << "\n--- Most Borrowed Books ---\n";
    auto topBooks = getMostBorrowedBooks(5);
    for (size_t i = 0; i < topBooks.size(); i++) {
        int bookID = topBooks[i].first;
        int count = topBooks[i].second;
        auto books = booksByTitle->searchByPredicate([bookID](const Book& b) {
            return b.getBookID() == bookID;
        });
        if (!books.empty()) {
            cout << "  " << books[0].getTitle() << " - " << count << " times\n";
        }
    }
    
    cout << "\n--- Most Active Users ---\n";
    auto topUsers = getMostActiveUsers(5);
    for (size_t i = 0; i < topUsers.size(); i++) {
        int userID = topUsers[i].first;
        int count = topUsers[i].second;
        auto user = usersByID.find(userID);
        if (user.has_value()) {
            cout << "  " << user.value().getName() << " - " << count << " books borrowed\n";
        }
    }
    
    cout << "=========================================\n\n";
}

int Library::getTotalBooks() const {
    return booksByTitle->getAllElements().size();
}

int Library::getTotalUsers() const {
    return usersByID.getSize();
}

vector<Book> Library::getAllBooks() const {
    return booksByTitle->getAllElements();
}

vector<User> Library::getAllUsers() const {
    return usersByID.getAllValues();
}

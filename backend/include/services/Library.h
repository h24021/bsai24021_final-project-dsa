#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../models/Book.h"
#include "../models/User.h"
#include "../data_structures/BTree.h"
#include "../data_structures/HashTable.h"

using namespace std;

class Library {
private:
    // B-Tree for efficient book storage and search (degree 3)
    BTree<Book>* booksByTitle;
    
    // Hash tables for O(1) user lookup
    HashTable<int, User> usersByID;      // Key: userID
    HashTable<string, User> usersByEmail; // Key: email
    
    // Statistics tracking
    HashTable<int, int> borrowCounts;     // Book ID -> borrow count
    
public:
    Library();
    ~Library();
    
    // Book operations
    void addBook(const Book& b);
    void printAllBooks();
    
    // Search operations using B-Tree
    vector<Book> searchBookByTitle(const string& title);
    vector<Book> searchBookByAuthor(const string& author);
    vector<Book> searchBookByCategory(const string& category);
    Book* findBookByID(int bookID);
    
    // User operations
    void addUser(const User& u);
    User* findUserByID(int userID);
    User* findUserByEmail(const string& email);
    void printAllUsers();
    
    // Borrow/Return operations
    bool borrowBook(int userID, int bookID);
    bool returnBook(int userID, int bookID);
    
    // Admin Dashboard - Statistics
    vector<pair<int, int>> getMostBorrowedBooks(int topN = 5);
    vector<pair<int, int>> getMostActiveUsers(int topN = 5);
    void printStatistics();
    
    // Utility
    int getTotalBooks() const;
    int getTotalUsers() const;
};

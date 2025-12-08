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
     
    BTree<Book>* booksByTitle;

    HashTable<int, User> usersByID;       
    HashTable<string, User> usersByEmail;  

    HashTable<int, int> borrowCounts;      
    
public:
    Library();
    ~Library();

    void addBook(const Book& b);
    void printAllBooks();

    vector<Book> searchBookByTitle(const string& title);
    vector<Book> searchBookByAuthor(const string& author);
    vector<Book> searchBookByCategory(const string& category);
    Book* findBookByID(int bookID);

    void addUser(const User& u);
    User* findUserByID(int userID);
    User* findUserByEmail(const string& email);
    void printAllUsers();

    bool borrowBook(int userID, int bookID);
    bool returnBook(int userID, int bookID);

    vector<pair<int, int>> getMostBorrowedBooks(int topN = 5);
    vector<pair<int, int>> getMostActiveUsers(int topN = 5);
    void printStatistics();

    vector<Book> getAllBooks() const;
    vector<User> getAllUsers() const;
    int getTotalBooks() const;
    int getTotalUsers() const;
};

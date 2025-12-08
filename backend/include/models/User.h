#pragma once
#include <iostream>
#include <vector>
using namespace std;

class User {
private:
    int userID;
    string name;
    string email;
    string role;
    int borrowedBooks;
    vector<int> borrowedBookIDs;

public:
    User();
    User(int id, string n, string e, string r);

    void printUser() const;
    
    // Getters
    int getUserID() const;
    string getName() const;
    string getEmail() const;
    string getRole() const;
    int getBorrowedBooksCount() const;
    vector<int> getBorrowedBookIDs() const;
    
    // Borrow/Return operations
    bool borrowBook(int bookID);
    bool returnBook(int bookID);
    bool hasBorrowedBook(int bookID) const;
};

#pragma once
#include <iostream>
#include <vector>
using namespace std;

class User {
private:
    int userID;
    string username;
    string name;
    string email;
    string password;
    string role; 
    int borrowedBooks;
    vector<int> borrowedBookIDs;

public:
    User();
    User(int id, string uname, string n, string e, string pwd, string r);

    void printUser() const;

    int getUserID() const;
    string getUsername() const;
    string getName() const;
    string getEmail() const;
    string getPassword() const;
    string getRole() const;
    int getBorrowedBooksCount() const;
    vector<int> getBorrowedBookIDs() const;

    void setPassword(string pwd);
    bool isAdmin() const;
    
    bool borrowBook(int bookID);
    bool returnBook(int bookID);
    bool hasBorrowedBook(int bookID) const;
};

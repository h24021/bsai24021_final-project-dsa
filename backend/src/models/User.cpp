#include "../../include/models/User.h"
#include <algorithm>

User::User() : userID(0), name(""), email(""), role(""), borrowedBooks(0) {}

User::User(int id, string n, string e, string r) {
    userID = id;
    name = n;
    email = e;
    role = r;
    borrowedBooks = 0;
}

void User::printUser() const {
    cout << "User ID: " << userID 
         << ", Name: " << name 
         << ", Email: " << email
         << ", Role: " << role 
         << ", Borrowed Books: " << borrowedBooks << endl;
}

// Getters
int User::getUserID() const { return userID; }
string User::getName() const { return name; }
string User::getEmail() const { return email; }
string User::getRole() const { return role; }
int User::getBorrowedBooksCount() const { return borrowedBooks; }
vector<int> User::getBorrowedBookIDs() const { return borrowedBookIDs; }

// Borrow/Return operations
bool User::borrowBook(int bookID) {
    if (!hasBorrowedBook(bookID)) {
        borrowedBookIDs.push_back(bookID);
        borrowedBooks++;
        return true;
    }
    return false;
}

bool User::returnBook(int bookID) {
    auto it = find(borrowedBookIDs.begin(), borrowedBookIDs.end(), bookID);
    if (it != borrowedBookIDs.end()) {
        borrowedBookIDs.erase(it);
        borrowedBooks--;
        return true;
    }
    return false;
}

bool User::hasBorrowedBook(int bookID) const {
    return find(borrowedBookIDs.begin(), borrowedBookIDs.end(), bookID) != borrowedBookIDs.end();
}

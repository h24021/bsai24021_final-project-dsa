#include "../../include/models/User.h"
#include <algorithm>

User::User() : userID(0), username(""), name(""), email(""), password(""), role("USER"), borrowedBooks(0) {}

User::User(int id, string uname, string n, string e, string pwd, string r) {
    userID = id;
    username = uname;
    name = n;
    email = e;
    password = pwd;
    role = r;
    borrowedBooks = 0;
}

void User::printUser() const {
    cout << "User ID: " << userID
         << ", Username: " << username
         << ", Name: " << name
         << ", Email: " << email
         << ", Role: " << role
         << ", Borrowed Books: " << borrowedBooks << endl;
}

int User::getUserID() const { return userID; }
string User::getUsername() const { return username; }
string User::getName() const { return name; }
string User::getEmail() const { return email; }
string User::getPassword() const { return password; }
string User::getRole() const { return role; }
int User::getBorrowedBooksCount() const { return borrowedBooks; }
vector<int> User::getBorrowedBookIDs() const { return borrowedBookIDs; }

void User::setPassword(string pwd) { password = pwd; }

bool User::isAdmin() const { 
    return role == "ADMIN"; 
}

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

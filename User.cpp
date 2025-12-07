#include "User.h"

User::User() {}

User::User(int id, string n, string r) {
    userID = id;
    name = n;
    role = r;
    borrowedBooks = 0;
}

void User::printUser() {
    cout << "User ID: " << userID 
         << ", Name: " << name 
         << ", Role: " << role << endl;
}

int User::getUserID() { return userID; }

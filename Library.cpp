#include "Library.h"
#include <iostream>
using namespace std;

void Library::addBook(Book b) {
    books.push_back(b);
    cout << "Book added: " << b.getTitle() << endl;
}

void Library::addUser(User u) {
    users.push_back(u);
    cout << "User added: " << u.getUserID() << endl;
}

void Library::printAllBooks() {
    for (auto &b : books) {
        b.printBook();
    }
}

void Library::searchBookByISBN(string isbn) {
    for (auto &b : books) {
        if (b.getISBN() == isbn) {
            b.printBook();
            return;
        }
    }
    cout << "Book not found." << endl;
}

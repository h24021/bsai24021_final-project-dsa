#include "../../include/models/Book.h"

Book::Book() {}

Book::Book(int id, string t, string a, string i, string c, int cp, int av) {
    bookID = id;
    title = t;
    author = a;
    isbn = i;
    category = c;
    copies = cp;
    availableCopies = av;
}

void Book::printBook() {
    cout << "Book ID: " << bookID << ", Title: " << title
         << ", Author: " << author << ", ISBN: " << isbn << endl;
}

string Book::getISBN() const { return isbn; }
int Book::getBookID() const { return bookID; }
string Book::getTitle() const { return title; }

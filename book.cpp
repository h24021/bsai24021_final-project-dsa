#include "book.h"

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

string Book::getISBN() { return isbn; }
int Book::getBookID() { return bookID; }
string Book::getTitle() { return title; }

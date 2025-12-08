#include "../../include/models/Book.h"

Book::Book() : bookID(0), title(""), author(""), isbn(""), category(""), copies(0), availableCopies(0) {}

Book::Book(int id, string t, string a, string i, string c, int cp, int av) {
    bookID = id;
    title = t;
    author = a;
    isbn = i;
    category = c;
    copies = cp;
    availableCopies = av;
}

void Book::printBook() const {
    cout << "Book ID: " << bookID << ", Title: " << title
         << ", Author: " << author << ", ISBN: " << isbn 
         << ", Available: " << availableCopies << "/" << copies << endl;
}

string Book::getISBN() const { return isbn; }
int Book::getBookID() const { return bookID; }
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getCategory() const { return category; }
int Book::getCopies() const { return copies; }
int Book::getAvailableCopies() const { return availableCopies; }

void Book::setAvailableCopies(int count) {
    if (count >= 0 && count <= copies) {
        availableCopies = count;
    }
}

bool Book::borrowBook() {
    if (availableCopies > 0) {
        availableCopies--;
        return true;
    }
    return false;
}

bool Book::returnBook() {
    if (availableCopies < copies) {
        availableCopies++;
        return true;
    }
    return false;
}

bool Book::operator<(const Book& other) const {
    string t1 = title, t2 = other.title;
    transform(t1.begin(), t1.end(), t1.begin(), ::tolower);
    transform(t2.begin(), t2.end(), t2.begin(), ::tolower);
    return t1 < t2;
}

bool Book::operator>(const Book& other) const {
    return other < *this;
}

bool Book::operator==(const Book& other) const {
    string t1 = title, t2 = other.title;
    transform(t1.begin(), t1.end(), t1.begin(), ::tolower);
    transform(t2.begin(), t2.end(), t2.begin(), ::tolower);
    return t1 == t2;
}

bool Book::operator<=(const Book& other) const {
    return !(*this > other);
}

bool Book::operator>=(const Book& other) const {
    return !(*this < other);
}

int Book::compareByTitle(const Book& a, const Book& b) {
    string t1 = a.title, t2 = b.title;
    transform(t1.begin(), t1.end(), t1.begin(), ::tolower);
    transform(t2.begin(), t2.end(), t2.begin(), ::tolower);
    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

int Book::compareByAuthor(const Book& a, const Book& b) {
    string a1 = a.author, a2 = b.author;
    transform(a1.begin(), a1.end(), a1.begin(), ::tolower);
    transform(a2.begin(), a2.end(), a2.begin(), ::tolower);
    if (a1 < a2) return -1;
    if (a1 > a2) return 1;
    return 0;
}

int Book::compareByISBN(const Book& a, const Book& b) {
    if (a.isbn < b.isbn) return -1;
    if (a.isbn > b.isbn) return 1;
    return 0;
}

int Book::compareByID(const Book& a, const Book& b) {
    if (a.bookID < b.bookID) return -1;
    if (a.bookID > b.bookID) return 1;
    return 0;
}

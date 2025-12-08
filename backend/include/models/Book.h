#pragma once
#include <iostream>
#include <algorithm>
using namespace std;

class Book {
private:
    int bookID;
    string title;
    string author;
    string isbn;
    string category;
    int copies;
    int availableCopies;

public:
    Book();
    Book(int id, string t, string a, string i, string c, int cp, int av);

    void printBook() const;

    string getISBN() const;
    int getBookID() const;
    string getTitle() const;
    string getAuthor() const;
    string getCategory() const;
    int getCopies() const;
    int getAvailableCopies() const;

    void setAvailableCopies(int count);
    bool borrowBook();
    bool returnBook();

    bool operator<(const Book& other) const;
    bool operator>(const Book& other) const;
    bool operator==(const Book& other) const;
    bool operator<=(const Book& other) const;
    bool operator>=(const Book& other) const;

    static int compareByTitle(const Book& a, const Book& b);
    static int compareByAuthor(const Book& a, const Book& b);
    static int compareByISBN(const Book& a, const Book& b);
    static int compareByID(const Book& a, const Book& b);
};

#pragma once
#include <iostream>
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

    void printBook();
    string getISBN() const;
    int getBookID() const;
    string getTitle() const;
};

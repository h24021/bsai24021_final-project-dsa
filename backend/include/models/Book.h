#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
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
    string coverImage;
    string type;
    vector<string> downloadLinks;

public:
    Book();
    Book(int id, string t, string a, string i, string c, int cp, int av, string cover = "", string tp = "", vector<string> dl = {});

    void printBook() const;

    string getISBN() const;
    int getBookID() const;
    string getTitle() const;
    string getAuthor() const;
    string getCategory() const;
    int getCopies() const;
    int getAvailableCopies() const;
    string getCoverImage() const;
    string getType() const;
    vector<string> getDownloadLinks() const;

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

#pragma once
#include <vector>
#include "../models/Book.h"
#include "../models/User.h"

class Library {
private:
    vector<Book> books;
    vector<User> users;

public:
    void addBook(Book b);
    void addUser(User u);
    void printAllBooks();
    void searchBookByISBN(string isbn);
};

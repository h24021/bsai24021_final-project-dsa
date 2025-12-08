#include <iostream>
#include "../include/services/Library.h"
using namespace std;

int main() {
    Library lib;

    lib.addBook(Book(1, "The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", "Fiction", 3, 3));
    lib.addBook(Book(2, "1984", "George Orwell", "9780451524935", "Fiction", 5, 5));

    lib.addUser(User(1, "Ali Khan", "Student"));
    lib.addUser(User(2, "Sara Ahmed", "Librarian"));

    lib.searchBookByISBN("9780743273565");

    return 0;
}

#include <iostream>
#include "../include/services/Library.h"
using namespace std;

void displayMenu() {
    cout << "\n========================================\n";
    cout << "   LIBRARY MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1.  Add Book\n";
    cout << "2.  Add User\n";
    cout << "3.  Search Book by Title\n";
    cout << "4.  Search Book by Author\n";
    cout << "5.  Search Book by Category\n";
    cout << "6.  Find User by ID\n";
    cout << "7.  Find User by Email\n";
    cout << "8.  Borrow Book\n";
    cout << "9.  Return Book\n";
    cout << "10. Display All Books\n";
    cout << "11. Display All Users\n";
    cout << "12. View Statistics\n";
    cout << "0.  Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

void demoLibrarySystem() {
    Library lib;
    
    cout << "\n========== DEMO: Library Management System ==========\n\n";
    
    // Add sample books
    cout << ">> Adding Books to Library...\n";
    lib.addBook(Book(1, "The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", "Fiction", 3, 3));
    lib.addBook(Book(2, "1984", "George Orwell", "9780451524935", "Dystopian", 5, 5));
    lib.addBook(Book(3, "To Kill a Mockingbird", "Harper Lee", "9780061120084", "Fiction", 4, 4));
    lib.addBook(Book(4, "Animal Farm", "George Orwell", "9780451526342", "Satire", 3, 3));
    lib.addBook(Book(5, "Pride and Prejudice", "Jane Austen", "9780141439518", "Romance", 2, 2));
    lib.addBook(Book(6, "The Catcher in the Rye", "J.D. Salinger", "9780316769174", "Fiction", 4, 4));
    lib.addBook(Book(7, "Brave New World", "Aldous Huxley", "9780060850524", "Dystopian", 3, 3));
    
    // Add sample users
    cout << "\n>> Adding Users to Library...\n";
    lib.addUser(User(101, "Alice Smith", "alice@example.com", "Student"));
    lib.addUser(User(102, "Bob Jones", "bob@example.com", "Faculty"));
    lib.addUser(User(103, "Charlie Brown", "charlie@example.com", "Student"));
    lib.addUser(User(104, "Diana Prince", "diana@example.com", "Librarian"));
    
    // Display all books
    cout << "\n>> Displaying All Books:\n";
    lib.printAllBooks();
    
    // Search by title
    cout << "\n>> Searching for books with 'the' in title:\n";
    auto titleResults = lib.searchBookByTitle("the");
    cout << "Found " << titleResults.size() << " books:\n";
    for (const auto& book : titleResults) {
        book.printBook();
    }
    
    // Search by author
    cout << "\n>> Searching for books by 'George Orwell':\n";
    auto authorResults = lib.searchBookByAuthor("orwell");
    cout << "Found " << authorResults.size() << " books:\n";
    for (const auto& book : authorResults) {
        book.printBook();
    }
    
    // Search by category
    cout << "\n>> Searching for 'Dystopian' books:\n";
    auto categoryResults = lib.searchBookByCategory("dystopian");
    cout << "Found " << categoryResults.size() << " books:\n";
    for (const auto& book : categoryResults) {
        book.printBook();
    }
    
    // User lookup by ID
    cout << "\n>> Looking up user with ID 101:\n";
    User* user = lib.findUserByID(101);
    if (user) {
        user->printUser();
    }
    
    // User lookup by email
    cout << "\n>> Looking up user with email 'bob@example.com':\n";
    user = lib.findUserByEmail("bob@example.com");
    if (user) {
        user->printUser();
    }
    
    // Borrow books
    cout << "\n>> Testing Borrow Operations:\n";
    lib.borrowBook(101, 1); // Alice borrows The Great Gatsby
    lib.borrowBook(101, 2); // Alice borrows 1984
    lib.borrowBook(102, 2); // Bob borrows 1984
    lib.borrowBook(103, 4); // Charlie borrows Animal Farm
    lib.borrowBook(102, 5); // Bob borrows Pride and Prejudice
    
    cout << "\n>> Attempting to borrow same book twice:\n";
    lib.borrowBook(101, 1); // Should fail
    
    // Display statistics
    cout << "\n>> Library Statistics:\n";
    lib.printStatistics();
    
    // Return a book
    cout << "\n>> Returning Book:\n";
    lib.returnBook(101, 1); // Alice returns The Great Gatsby
    
    cout << "\n>> Updated Statistics:\n";
    lib.printStatistics();
    
    // Display all users
    cout << "\n>> All Users:\n";
    lib.printAllUsers();
    
    cout << "\n========== Demo Complete ==========\n\n";
}

int main() {
    // Run the demo
    demoLibrarySystem();
    
    return 0;
}


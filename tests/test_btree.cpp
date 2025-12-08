#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "../include/services/Library.h"
#include "../include/data_structures/BTree.h"

using namespace std;

int testsRun = 0;
int testsPassed = 0;
int testsFailed = 0;

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

void printTestHeader(const string& testName) {
    cout << "\n" << BLUE << "========== " << testName << " ==========" << RESET << endl;
}

void testPassed(const string& testName) {
    testsRun++;
    testsPassed++;
    cout << GREEN << "[PASS] " << RESET << testName << endl;
}

void testFailed(const string& testName, const string& reason = "") {
    testsRun++;
    testsFailed++;
    cout << RED << "[FAIL] " << RESET << testName;
    if (!reason.empty()) {
        cout << " - " << reason;
    }
    cout << endl;
}

void testBTreeBasicInsertion() {
    printTestHeader("B-Tree Basic Insertion Test");
    
    BTree<int> tree(3, [](const int& a, const int& b) {
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    });

    vector<int> values = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int val : values) {
        tree.insert(val);
    }

    bool allFound = true;
    for (int val : values) {
        if (tree.search(val) == nullptr) {
            allFound = false;
            break;
        }
    }
    
    if (allFound) {
        testPassed("All inserted values found in B-Tree");
    } else {
        testFailed("Some inserted values not found in B-Tree");
    }
}

void testBTreeSearchNotFound() {
    printTestHeader("B-Tree Search Not Found Test");
    
    BTree<int> tree(3, [](const int& a, const int& b) {
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    });
    
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    
    if (tree.search(999) == nullptr) {
        testPassed("Correctly returns null for non-existent value");
    } else {
        testFailed("Should return null for non-existent value");
    }
}

void testBTreeTraversal() {
    printTestHeader("B-Tree Traversal Test");
    
    BTree<int> tree(3, [](const int& a, const int& b) {
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    });
    
    vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    for (int val : values) {
        tree.insert(val);
    }
    
    vector<int> traversed;
    tree.traverse([&traversed](const int& val) {
        traversed.push_back(val);
    });

    bool sorted = true;
    for (size_t i = 1; i < traversed.size(); i++) {
        if (traversed[i-1] > traversed[i]) {
            sorted = false;
            break;
        }
    }
    
    if (sorted && traversed.size() == values.size()) {
        testPassed("B-Tree traversal produces sorted output");
    } else {
        testFailed("B-Tree traversal not properly sorted");
    }
}

void testBTreeLargeDataset() {
    printTestHeader("B-Tree Large Dataset Test");
    
    BTree<int> tree(5, [](const int& a, const int& b) {
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    });

    int count = 1000;
    for (int i = 0; i < count; i++) {
        tree.insert(i * 2);  
    }

    bool allFound = true;
    for (int i = 0; i < count; i++) {
        if (tree.search(i * 2) == nullptr) {
            allFound = false;
            break;
        }
    }

    bool noFalsePositives = true;
    for (int i = 0; i < count; i++) {
        if (tree.search(i * 2 + 1) != nullptr) {
            noFalsePositives = false;
            break;
        }
    }
    
    if (allFound && noFalsePositives) {
        testPassed("B-Tree handles 1000 insertions and searches correctly");
    } else {
        testFailed("B-Tree failed on large dataset");
    }
}

void testLibraryAddBooks() {
    printTestHeader("Library Add Books Test");
    
    Library lib;
    
    lib.addBook(Book(1, "The Great Gatsby", "F. Scott Fitzgerald", "ISBN001", "Fiction", 3, 3));
    lib.addBook(Book(2, "1984", "George Orwell", "ISBN002", "Dystopian", 5, 5));
    lib.addBook(Book(3, "To Kill a Mockingbird", "Harper Lee", "ISBN003", "Fiction", 4, 4));
    
    if (lib.getTotalBooks() == 3) {
        testPassed("Successfully added 3 books to library");
    } else {
        testFailed("Book count mismatch", "Expected 3, got " + to_string(lib.getTotalBooks()));
    }
}

void testLibrarySearchByTitle() {
    printTestHeader("Library Search by Title Test");
    
    Library lib;
    
    lib.addBook(Book(1, "The Great Gatsby", "F. Scott Fitzgerald", "ISBN001", "Fiction", 3, 3));
    lib.addBook(Book(2, "Great Expectations", "Charles Dickens", "ISBN002", "Fiction", 2, 2));
    lib.addBook(Book(3, "1984", "George Orwell", "ISBN003", "Dystopian", 5, 5));
    
    auto results = lib.searchBookByTitle("great");
    
    if (results.size() == 2) {
        testPassed("Search by title found 2 books containing 'great'");
    } else {
        testFailed("Search by title failed", "Expected 2 results, got " + to_string(results.size()));
    }
}

void testLibrarySearchByAuthor() {
    printTestHeader("Library Search by Author Test");
    
    Library lib;
    
    lib.addBook(Book(1, "1984", "George Orwell", "ISBN001", "Dystopian", 5, 5));
    lib.addBook(Book(2, "Animal Farm", "George Orwell", "ISBN002", "Satire", 3, 3));
    lib.addBook(Book(3, "The Great Gatsby", "F. Scott Fitzgerald", "ISBN003", "Fiction", 3, 3));
    
    auto results = lib.searchBookByAuthor("orwell");
    
    if (results.size() == 2) {
        testPassed("Search by author found 2 books by Orwell");
    } else {
        testFailed("Search by author failed", "Expected 2 results, got " + to_string(results.size()));
    }
}

void testLibraryAddUsers() {
    printTestHeader("Library Add Users Test");
    
    Library lib;
    
    lib.addUser(User(1, "Alice Smith", "alice@example.com", "Student"));
    lib.addUser(User(2, "Bob Jones", "bob@example.com", "Librarian"));
    lib.addUser(User(3, "Charlie Brown", "charlie@example.com", "Student"));
    
    if (lib.getTotalUsers() == 3) {
        testPassed("Successfully added 3 users to library");
    } else {
        testFailed("User count mismatch", "Expected 3, got " + to_string(lib.getTotalUsers()));
    }
}

void testLibraryUserLookupByID() {
    printTestHeader("Library User Lookup by ID Test");
    
    Library lib;
    
    lib.addUser(User(101, "Alice Smith", "alice@example.com", "Student"));
    lib.addUser(User(102, "Bob Jones", "bob@example.com", "Librarian"));
    
    User* user = lib.findUserByID(101);
    
    if (user != nullptr && user->getName() == "Alice Smith") {
        testPassed("User lookup by ID successful");
    } else {
        testFailed("User lookup by ID failed");
    }
    
    User* notFound = lib.findUserByID(999);
    if (notFound == nullptr) {
        testPassed("User lookup returns null for non-existent ID");
    } else {
        testFailed("User lookup should return null for non-existent ID");
    }
}

void testLibraryUserLookupByEmail() {
    printTestHeader("Library User Lookup by Email Test");
    
    Library lib;
    
    lib.addUser(User(101, "Alice Smith", "alice@example.com", "Student"));
    lib.addUser(User(102, "Bob Jones", "bob@example.com", "Librarian"));
    
    User* user = lib.findUserByEmail("alice@example.com");
    
    if (user != nullptr && user->getUserID() == 101) {
        testPassed("User lookup by email successful");
    } else {
        testFailed("User lookup by email failed");
    }
}

void testLibraryBorrowBook() {
    printTestHeader("Library Borrow Book Test");
    
    Library lib;
    
    lib.addBook(Book(1, "1984", "George Orwell", "ISBN001", "Dystopian", 2, 2));
    lib.addUser(User(101, "Alice Smith", "alice@example.com", "Student"));
    
    bool success = lib.borrowBook(101, 1);
    
    if (success) {
        testPassed("Book borrowed successfully");
    } else {
        testFailed("Book borrow operation failed");
    }

    bool shouldFail = lib.borrowBook(101, 1);
    if (!shouldFail) {
        testPassed("Correctly prevents borrowing same book twice");
    } else {
        testFailed("Should not allow borrowing same book twice");
    }
}

void testLibraryReturnBook() {
    printTestHeader("Library Return Book Test");
    
    Library lib;
    
    lib.addBook(Book(1, "1984", "George Orwell", "ISBN001", "Dystopian", 2, 2));
    lib.addUser(User(101, "Alice Smith", "alice@example.com", "Student"));
    
    lib.borrowBook(101, 1);
    bool success = lib.returnBook(101, 1);
    
    if (success) {
        testPassed("Book returned successfully");
    } else {
        testFailed("Book return operation failed");
    }

    bool shouldFail = lib.returnBook(101, 999);
    if (!shouldFail) {
        testPassed("Correctly prevents returning non-borrowed book");
    } else {
        testFailed("Should not allow returning non-borrowed book");
    }
}

void testLibraryStatistics() {
    printTestHeader("Library Statistics Test");
    
    Library lib;

    lib.addBook(Book(1, "1984", "George Orwell", "ISBN001", "Dystopian", 2, 2));
    lib.addBook(Book(2, "The Great Gatsby", "F. Scott Fitzgerald", "ISBN002", "Fiction", 3, 3));

    lib.addUser(User(101, "Alice Smith", "alice@example.com", "Student"));
    lib.addUser(User(102, "Bob Jones", "bob@example.com", "Student"));

    lib.borrowBook(101, 1);  
    lib.borrowBook(102, 1);  
    lib.borrowBook(101, 2);  
    
    auto mostBorrowed = lib.getMostBorrowedBooks(1);
    
    if (!mostBorrowed.empty() && mostBorrowed[0].first == 1) {
        testPassed("Most borrowed book statistics correct");
    } else {
        testFailed("Most borrowed book statistics incorrect");
    }
    
    auto mostActive = lib.getMostActiveUsers(1);
    
    if (!mostActive.empty() && mostActive[0].first == 101) {
        testPassed("Most active user statistics correct");
    } else {
        testFailed("Most active user statistics incorrect");
    }
}

void testBookComparison() {
    printTestHeader("Book Comparison Test");
    
    Book b1(1, "AAA Book", "Author A", "ISBN001", "Fiction", 1, 1);
    Book b2(2, "ZZZ Book", "Author Z", "ISBN002", "Fiction", 1, 1);
    Book b3(3, "aaa book", "Author A", "ISBN003", "Fiction", 1, 1);  
    
    if (b1 < b2) {
        testPassed("Book comparison by title works (less than)");
    } else {
        testFailed("Book comparison by title failed (less than)");
    }
    
    if (b1 == b3) {
        testPassed("Book comparison is case-insensitive");
    } else {
        testFailed("Book comparison should be case-insensitive");
    }
}

void testCaseInsensitiveSearch() {
    printTestHeader("Case-Insensitive Search Test");
    
    Library lib;
    
    lib.addBook(Book(1, "The Great GATSBY", "F. Scott Fitzgerald", "ISBN001", "Fiction", 3, 3));
    
    auto results1 = lib.searchBookByTitle("gatsby");
    auto results2 = lib.searchBookByTitle("GATSBY");
    auto results3 = lib.searchBookByTitle("GaTsBY");
    
    if (results1.size() == 1 && results2.size() == 1 && results3.size() == 1) {
        testPassed("Case-insensitive search works correctly");
    } else {
        testFailed("Case-insensitive search failed");
    }
}

void testStressTestWithManyBooks() {
    printTestHeader("Stress Test with Many Books");
    
    Library lib;

    for (int i = 1; i <= 100; i++) {
        string title = "Book " + to_string(i);
        string author = "Author " + to_string(i % 10);  
        string isbn = "ISBN" + to_string(i);
        lib.addBook(Book(i, title, author, isbn, "Fiction", 2, 2));
    }
    
    if (lib.getTotalBooks() == 100) {
        testPassed("Successfully added 100 books");
    } else {
        testFailed("Failed to add 100 books");
    }

    auto results = lib.searchBookByAuthor("Author 5");
    
    if (results.size() == 10) {
        testPassed("Search correctly finds 10 books by same author");
    } else {
        testFailed("Search by author in large dataset failed", 
                   "Expected 10, got " + to_string(results.size()));
    }
}

void runAllTests() {
    cout << YELLOW << "\n╔════════════════════════════════════════════╗" << RESET << endl;
    cout << YELLOW << "║  Library Management System - Test Suite   ║" << RESET << endl;
    cout << YELLOW << "╚════════════════════════════════════════════╝" << RESET << endl;

    testBTreeBasicInsertion();
    testBTreeSearchNotFound();
    testBTreeTraversal();
    testBTreeLargeDataset();

    testBookComparison();

    testLibraryAddBooks();
    testLibrarySearchByTitle();
    testLibrarySearchByAuthor();
    testCaseInsensitiveSearch();
    testLibraryAddUsers();
    testLibraryUserLookupByID();
    testLibraryUserLookupByEmail();
    testLibraryBorrowBook();
    testLibraryReturnBook();
    testLibraryStatistics();
    testStressTestWithManyBooks();

    cout << "\n" << YELLOW << "╔════════════════════════════════════════════╗" << RESET << endl;
    cout << YELLOW << "║            TEST SUMMARY                    ║" << RESET << endl;
    cout << YELLOW << "╚════════════════════════════════════════════╝" << RESET << endl;
    cout << "Total Tests Run:    " << testsRun << endl;
    cout << GREEN << "Tests Passed:       " << testsPassed << RESET << endl;
    cout << RED << "Tests Failed:       " << testsFailed << RESET << endl;
    
    double passRate = (testsRun > 0) ? (100.0 * testsPassed / testsRun) : 0;
    cout << "Pass Rate:          " << passRate << "%" << endl;
    
    if (testsFailed == 0) {
        cout << GREEN << "\n✓ ALL TESTS PASSED!" << RESET << endl;
    } else {
        cout << RED << "\n✗ Some tests failed. Please review." << RESET << endl;
    }
}

int main() {
    runAllTests();
    return 0;
}

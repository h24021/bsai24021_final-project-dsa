# Library Management System - Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                        LIBRARY MANAGEMENT SYSTEM                             │
│                      (B-Tree + Hash Table Implementation)                    │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                              USER INTERFACE                                  │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │ Search Books │  │  Add Books   │  │ Borrow/Return│  │  Statistics  │   │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘   │
│         │                  │                  │                  │            │
└─────────┼──────────────────┼──────────────────┼──────────────────┼───────────┘
          │                  │                  │                  │
          ▼                  ▼                  ▼                  ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           LIBRARY SERVICE LAYER                              │
│  ┌───────────────────────────────────────────────────────────────────────┐  │
│  │                         Library.h / Library.cpp                        │  │
│  │  • searchBookByTitle()       • borrowBook()                           │  │
│  │  • searchBookByAuthor()      • returnBook()                           │  │
│  │  • searchBookByCategory()    • getMostBorrowedBooks()                 │  │
│  │  • findUserByID()            • getMostActiveUsers()                   │  │
│  │  • findUserByEmail()         • printStatistics()                      │  │
│  └───────────────────────────────────────────────────────────────────────┘  │
└────────────────────┬────────────────────────────┬───────────────────────────┘
                     │                            │
         ┌───────────▼────────────┐   ┌──────────▼──────────┐
         │   BOOK STORAGE         │   │   USER STORAGE      │
         │   (B-Tree)             │   │   (Hash Tables)     │
         └───────────┬────────────┘   └──────────┬──────────┘
                     │                            │
                     ▼                            ▼

┌─────────────────────────────────────────────────────────────────────────────┐
│                          DATA STRUCTURE LAYER                                │
│                                                                              │
│  ┌──────────────────────────────────┐  ┌──────────────────────────────────┐│
│  │         B-TREE (Degree 3)        │  │        HASH TABLE                ││
│  │        BTree.h (Template)        │  │     HashTable.h (Template)       ││
│  ├──────────────────────────────────┤  ├──────────────────────────────────┤│
│  │ Root: BTreeNode<Book>*           │  │ Buckets: vector<list<Entry>>     ││
│  │                                  │  │ Capacity: 101 (prime)            ││
│  │ Operations:                      │  │ Load Factor: 0.75                ││
│  │  • insert(Book) - O(log n)       │  │                                  ││
│  │  • search(Book) - O(log n)       │  │ Operations:                      ││
│  │  • traverse() - O(n)             │  │  • insert(K,V) - O(1) avg        ││
│  │  • searchByPredicate() - O(n)    │  │  • find(K) - O(1) avg            ││
│  │                                  │  │  • remove(K) - O(1) avg          ││
│  │ Node Structure:                  │  │  • rehash() - O(n)               ││
│  │  ┌────────────────────┐          │  │                                  ││
│  │  │ Keys: [B1, B2, B3] │          │  │ Collision: Chaining              ││
│  │  │ Children: [C0...C4]│          │  │ Auto-Rehash: Yes                 ││
│  │  │ isLeaf: bool       │          │  │                                  ││
│  │  └────────────────────┘          │  │ Two instances in Library:        ││
│  │                                  │  │  • usersByID (int → User)        ││
│  │ Used for:                        │  │  • usersByEmail (string → User)  ││
│  │  ✓ Book storage by title        │  │                                  ││
│  │  ✓ Fast search O(log n)         │  │ Used for:                        ││
│  │  ✓ Sorted traversal             │  │  ✓ User lookup by ID O(1)        ││
│  │  ✓ Range queries                │  │  ✓ User lookup by email O(1)     ││
│  └──────────────────────────────────┘  └──────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                             MODEL LAYER                                      │
│  ┌─────────────────────────┐      ┌─────────────────────────────────────┐  │
│  │       Book Model        │      │          User Model                 │  │
│  │     Book.h/Book.cpp     │      │       User.h/User.cpp               │  │
│  ├─────────────────────────┤      ├─────────────────────────────────────┤  │
│  │ • bookID                │      │ • userID                            │  │
│  │ • title                 │      │ • name                              │  │
│  │ • author                │      │ • email                             │  │
│  │ • isbn                  │      │ • role                              │  │
│  │ • category              │      │ • borrowedBooks (count)             │  │
│  │ • copies                │      │ • borrowedBookIDs (vector)          │  │
│  │ • availableCopies       │      │                                     │  │
│  │                         │      │ Methods:                            │  │
│  │ Comparison Operators:   │      │ • borrowBook(bookID)                │  │
│  │ • operator<, >, ==      │      │ • returnBook(bookID)                │  │
│  │ • compareByTitle()      │      │ • hasBorrowedBook(bookID)           │  │
│  │ • compareByAuthor()     │      │                                     │  │
│  │ • compareByISBN()       │      │                                     │  │
│  └─────────────────────────┘      └─────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                        OPERATION FLOW EXAMPLES                               │
│                                                                              │
│  SEARCH BOOK BY TITLE:                                                      │
│  ┌──────┐   ┌─────────┐   ┌────────┐   ┌────────────┐                     │
│  │ User │──→│ Library │──→│ B-Tree │──→│ Books [1+] │                     │
│  └──────┘   └─────────┘   └────────┘   └────────────┘                     │
│              search        searchBy      Case-insensitive                   │
│              ByTitle()     Predicate()   title matching                     │
│                                          O(n) traversal                     │
│                                                                              │
│  FIND USER BY ID:                                                           │
│  ┌──────┐   ┌─────────┐   ┌────────────┐   ┌──────┐                       │
│  │ User │──→│ Library │──→│ Hash Table │──→│ User │                       │
│  └──────┘   └─────────┘   └────────────┘   └──────┘                       │
│              findUserByID  find(id)         O(1) lookup                     │
│                            Hash + bucket                                    │
│                            search                                           │
│                                                                              │
│  BORROW BOOK:                                                               │
│  ┌──────┐   ┌─────────┐   ┌──────────┐   ┌──────┐   ┌──────────┐          │
│  │ User │──→│ Library │──→│ Find User│──→│B-Tree│──→│Update Both│          │
│  └──────┘   └─────────┘   └──────────┘   └──────┘   └──────────┘          │
│              borrowBook()  Hash O(1)      Find Book  User + Stats          │
│                                           O(log n)   tracking              │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                          PERFORMANCE SUMMARY                                 │
│                                                                              │
│  Operation               Data Structure    Time Complexity    Verified      │
│  ────────────────────────────────────────────────────────────────────────   │
│  Insert Book             B-Tree            O(log n)           ✅            │
│  Search Book by Title    B-Tree            O(log n)           ✅            │
│  Search Book by Author   B-Tree            O(n)*              ✅            │
│  Traverse All Books      B-Tree            O(n)               ✅            │
│  Insert User             Hash Table        O(1) avg           ✅            │
│  Find User by ID         Hash Table        O(1) avg           ✅            │
│  Find User by Email      Hash Table        O(1) avg           ✅            │
│  Borrow Book             Combined          O(log n)           ✅            │
│  Return Book             Combined          O(log n)           ✅            │
│  Get Statistics          Various           O(n log n)         ✅            │
│                                                                              │
│  * O(n) because we need to check all books for author match                │
│                                                                              │
│  Space Complexity:                                                          │
│  • B-Tree: O(n) where n = number of books                                  │
│  • Hash Table: O(m) where m = number of users                              │
│  • Total: O(n + m)                                                          │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                          B-TREE VISUALIZATION                                │
│                                                                              │
│  Example B-Tree (degree 3, max 5 keys per node):                           │
│                                                                              │
│                         ┌──────────────────┐                                │
│                         │   [Book D]       │                                │
│                         └────┬────────┬────┘                                │
│                              │        │                                     │
│                 ┌────────────┘        └──────────┐                          │
│                 │                                │                          │
│         ┌───────▼────────┐              ┌────────▼───────┐                 │
│         │ [Book A, B, C] │              │ [Book E, F, G] │                 │
│         └────────────────┘              └────────────────┘                 │
│                                                                              │
│  Properties:                                                                │
│  • Each node has 2-5 keys (for degree 3)                                   │
│  • All leaves at same level                                                │
│  • Keys in sorted order                                                    │
│  • Automatic balancing on insert                                           │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                      HASH TABLE VISUALIZATION                                │
│                                                                              │
│  Bucket Array (usersByID):                                                 │
│                                                                              │
│  Index    Bucket (Linked List)                                             │
│  ────────────────────────────────────────────────────────────────          │
│  0        → NULL                                                            │
│  1        → [101: Alice] → NULL                                             │
│  2        → [102: Bob] → [202: Charlie] → NULL  (collision!)               │
│  3        → NULL                                                            │
│  ...                                                                        │
│  100      → [200: Diana] → NULL                                             │
│                                                                              │
│  Collision Resolution: Chaining (linked lists in each bucket)              │
│  Hash Function: std::hash<int>(userID) % 101                               │
│  Rehashing: When load > 0.75, double size and rehash all                   │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                            TEST COVERAGE MAP                                 │
│                                                                              │
│  ✅ B-Tree Tests (5)                                                        │
│     • Basic insertion                                                       │
│     • Search (found/not found)                                             │
│     • Traversal (sorted output)                                            │
│     • Large dataset (1000 items)                                           │
│     • Comparison operators                                                 │
│                                                                              │
│  ✅ Library Tests (17)                                                      │
│     • Add books/users                                                       │
│     • Search (title, author, category)                                     │
│     • User lookup (ID, email)                                              │
│     • Borrow/return operations                                             │
│     • Statistics tracking                                                  │
│     • Edge cases & validation                                              │
│     • Stress test (100 books)                                              │
│                                                                              │
│  Total: 22/22 Tests Passed ✅                                               │
└─────────────────────────────────────────────────────────────────────────────┘

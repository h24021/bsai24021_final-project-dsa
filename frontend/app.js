// Library Management System - Frontend JavaScript
// API Base URL - Since we're using a simulated backend, we'll create mock data

const API_BASE = '/api/v1'; // This would be your actual API endpoint
let transactions = [];
let mockData = {
    books: [],
    users: [],
    stats: {}
};

// Initialize the application
document.addEventListener('DOMContentLoaded', () => {
    initializeTabs();
    loadInitialData();
});

// Tab Navigation
function initializeTabs() {
    const tabBtns = document.querySelectorAll('.tab-btn');
    const tabContents = document.querySelectorAll('.tab-content');
    
    tabBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            // Remove active class from all tabs
            tabBtns.forEach(b => b.classList.remove('active'));
            tabContents.forEach(c => c.classList.remove('active'));
            
            // Add active class to clicked tab
            btn.classList.add('active');
            const tabId = btn.getAttribute('data-tab') + '-tab';
            document.getElementById(tabId).classList.add('active');
            
            // Load data for the active tab
            loadTabData(btn.getAttribute('data-tab'));
        });
    });
}

// Load initial data
function loadInitialData() {
    loadBooks();
    loadUsers();
    updateHeaderStats();
}

// Load data for specific tab
function loadTabData(tab) {
    switch(tab) {
        case 'books':
            loadBooks();
            break;
        case 'users':
            loadUsers();
            break;
        case 'statistics':
            loadStatistics();
            break;
    }
}

// ============ BOOKS OPERATIONS ============

async function loadBooks() {
    try {
        // Simulate API call - In production, use: fetch(API_BASE + '/books')
        const books = generateMockBooks();
        mockData.books = books;
        
        const tbody = document.getElementById('booksTableBody');
        if (books.length === 0) {
            tbody.innerHTML = '<tr><td colspan="6" class="no-data">No books found</td></tr>';
            return;
        }
        
        tbody.innerHTML = books.map(book => `
            <tr>
                <td>${book.bookID}</td>
                <td><strong>${book.title}</strong></td>
                <td>${book.author}</td>
                <td><span class="badge badge-info">${book.category}</span></td>
                <td>
                    <span class="availability ${book.availableCopies > 0 ? 'available' : 'unavailable'}">
                        ${book.availableCopies} / ${book.totalCopies}
                    </span>
                </td>
                <td>
                    <button class="btn btn-sm btn-info" onclick="viewBook(${book.bookID})">View</button>
                    <button class="btn btn-sm btn-warning" onclick="editBook(${book.bookID})">Edit</button>
                    <button class="btn btn-sm btn-danger" onclick="deleteBook(${book.bookID})">Delete</button>
                </td>
            </tr>
        `).join('');
        
        updateHeaderStats();
    } catch (error) {
        showToast('Error loading books: ' + error.message, 'error');
    }
}

function showAddBookForm() {
    document.getElementById('addBookForm').style.display = 'block';
}

function hideAddBookForm() {
    document.getElementById('addBookForm').style.display = 'none';
    document.getElementById('addBookForm').querySelector('form').reset();
}

async function addBook(event) {
    event.preventDefault();
    
    const bookData = {
        title: document.getElementById('bookTitle').value,
        author: document.getElementById('bookAuthor').value,
        isbn: document.getElementById('bookISBN').value || 'N/A',
        category: document.getElementById('bookCategory').value || 'General',
        totalCopies: parseInt(document.getElementById('bookCopies').value) || 1,
        availableCopies: parseInt(document.getElementById('bookAvailable').value) || 1
    };
    
    try {
        // Simulate API call - In production: fetch(API_BASE + '/books', {method: 'POST', body: JSON.stringify(bookData)})
        const newBook = {
            bookID: mockData.books.length > 0 ? Math.max(...mockData.books.map(b => b.bookID)) + 1 : 1000,
            ...bookData
        };
        
        mockData.books.push(newBook);
        
        hideAddBookForm();
        loadBooks();
        showToast(`Book "${bookData.title}" added successfully!`, 'success');
    } catch (error) {
        showToast('Error adding book: ' + error.message, 'error');
    }
}

function viewBook(bookID) {
    const book = mockData.books.find(b => b.bookID === bookID);
    if (book) {
        alert(`Book Details:\n\nID: ${book.bookID}\nTitle: ${book.title}\nAuthor: ${book.author}\nISBN: ${book.isbn}\nCategory: ${book.category}\nAvailable: ${book.availableCopies}/${book.totalCopies}`);
    }
}

function editBook(bookID) {
    showToast('Edit functionality - Connect to PUT /books/:id endpoint', 'info');
}

async function deleteBook(bookID) {
    if (!confirm('Are you sure you want to delete this book?')) return;
    
    try {
        // Simulate API call
        mockData.books = mockData.books.filter(b => b.bookID !== bookID);
        loadBooks();
        showToast('Book deleted successfully', 'success');
    } catch (error) {
        showToast('Error deleting book: ' + error.message, 'error');
    }
}

// ============ USERS OPERATIONS ============

async function loadUsers() {
    try {
        const users = generateMockUsers();
        mockData.users = users;
        
        const tbody = document.getElementById('usersTableBody');
        if (users.length === 0) {
            tbody.innerHTML = '<tr><td colspan="6" class="no-data">No users found</td></tr>';
            return;
        }
        
        tbody.innerHTML = users.map(user => `
            <tr>
                <td>${user.userID}</td>
                <td><strong>${user.name}</strong></td>
                <td>${user.email}</td>
                <td><span class="badge badge-${user.role === 'admin' ? 'danger' : user.role === 'librarian' ? 'warning' : 'success'}">${user.role}</span></td>
                <td>${user.borrowedBooks.length} books</td>
                <td>
                    <button class="btn btn-sm btn-info" onclick="viewUserBorrowedBooks(${user.userID})">Borrowed</button>
                    <button class="btn btn-sm btn-warning" onclick="editUser(${user.userID})">Edit</button>
                    <button class="btn btn-sm btn-danger" onclick="deleteUser(${user.userID})">Delete</button>
                </td>
            </tr>
        `).join('');
        
        updateHeaderStats();
    } catch (error) {
        showToast('Error loading users: ' + error.message, 'error');
    }
}

function showAddUserForm() {
    document.getElementById('addUserForm').style.display = 'block';
}

function hideAddUserForm() {
    document.getElementById('addUserForm').style.display = 'none';
    document.getElementById('addUserForm').querySelector('form').reset();
}

async function addUser(event) {
    event.preventDefault();
    
    const userData = {
        name: document.getElementById('userName').value,
        email: document.getElementById('userEmail').value,
        role: document.getElementById('userRole').value
    };
    
    try {
        const newUser = {
            userID: mockData.users.length > 0 ? Math.max(...mockData.users.map(u => u.userID)) + 1 : 5000,
            ...userData,
            borrowedBooks: []
        };
        
        mockData.users.push(newUser);
        
        hideAddUserForm();
        loadUsers();
        showToast(`User "${userData.name}" added successfully!`, 'success');
    } catch (error) {
        showToast('Error adding user: ' + error.message, 'error');
    }
}

function viewUserBorrowedBooks(userID) {
    const user = mockData.users.find(u => u.userID === userID);
    if (user) {
        if (user.borrowedBooks.length === 0) {
            alert(`${user.name} has no borrowed books.`);
        } else {
            const booksList = user.borrowedBooks.map(bookID => {
                const book = mockData.books.find(b => b.bookID === bookID);
                return book ? `- ${book.title} by ${book.author}` : `- Book ID: ${bookID}`;
            }).join('\n');
            alert(`${user.name}'s Borrowed Books:\n\n${booksList}`);
        }
    }
}

function editUser(userID) {
    showToast('Edit functionality - Connect to PUT /users/:id endpoint', 'info');
}

async function deleteUser(userID) {
    if (!confirm('Are you sure you want to delete this user?')) return;
    
    try {
        mockData.users = mockData.users.filter(u => u.userID !== userID);
        loadUsers();
        showToast('User deleted successfully', 'success');
    } catch (error) {
        showToast('Error deleting user: ' + error.message, 'error');
    }
}

// ============ BORROW/RETURN OPERATIONS ============

async function borrowBook(event) {
    event.preventDefault();
    
    const userID = parseInt(document.getElementById('borrowUserID').value);
    const bookID = parseInt(document.getElementById('borrowBookID').value);
    
    try {
        const user = mockData.users.find(u => u.userID === userID);
        const book = mockData.books.find(b => b.bookID === bookID);
        
        if (!user) {
            showToast('User not found!', 'error');
            return;
        }
        
        if (!book) {
            showToast('Book not found!', 'error');
            return;
        }
        
        if (book.availableCopies <= 0) {
            showToast('Book is not available!', 'error');
            return;
        }
        
        // Perform borrow
        book.availableCopies--;
        user.borrowedBooks.push(bookID);
        
        // Add to transaction history
        transactions.unshift({
            type: 'borrow',
            userID,
            userName: user.name,
            bookID,
            bookTitle: book.title,
            timestamp: new Date().toLocaleString()
        });
        
        updateTransactionHistory();
        loadBooks();
        updateHeaderStats();
        
        document.getElementById('borrowUserID').value = '';
        document.getElementById('borrowBookID').value = '';
        
        showToast(`${user.name} borrowed "${book.title}" successfully!`, 'success');
    } catch (error) {
        showToast('Error borrowing book: ' + error.message, 'error');
    }
}

async function returnBook(event) {
    event.preventDefault();
    
    const userID = parseInt(document.getElementById('returnUserID').value);
    const bookID = parseInt(document.getElementById('returnBookID').value);
    
    try {
        const user = mockData.users.find(u => u.userID === userID);
        const book = mockData.books.find(b => b.bookID === bookID);
        
        if (!user) {
            showToast('User not found!', 'error');
            return;
        }
        
        if (!book) {
            showToast('Book not found!', 'error');
            return;
        }
        
        const borrowedIndex = user.borrowedBooks.indexOf(bookID);
        if (borrowedIndex === -1) {
            showToast('User has not borrowed this book!', 'error');
            return;
        }
        
        // Perform return
        book.availableCopies++;
        user.borrowedBooks.splice(borrowedIndex, 1);
        
        // Add to transaction history
        transactions.unshift({
            type: 'return',
            userID,
            userName: user.name,
            bookID,
            bookTitle: book.title,
            timestamp: new Date().toLocaleString()
        });
        
        updateTransactionHistory();
        loadBooks();
        updateHeaderStats();
        
        document.getElementById('returnUserID').value = '';
        document.getElementById('returnBookID').value = '';
        
        showToast(`${user.name} returned "${book.title}" successfully!`, 'success');
    } catch (error) {
        showToast('Error returning book: ' + error.message, 'error');
    }
}

function updateTransactionHistory() {
    const historyDiv = document.getElementById('transactionHistory');
    
    if (transactions.length === 0) {
        historyDiv.innerHTML = '<p class="no-data">No transactions yet</p>';
        return;
    }
    
    historyDiv.innerHTML = transactions.slice(0, 10).map(t => `
        <div class="transaction-item ${t.type}">
            <div class="transaction-icon">${t.type === 'borrow' ? '📤' : '📥'}</div>
            <div class="transaction-details">
                <strong>${t.userName}</strong> ${t.type === 'borrow' ? 'borrowed' : 'returned'} 
                <strong>"${t.bookTitle}"</strong>
                <span class="transaction-time">${t.timestamp}</span>
            </div>
        </div>
    `).join('');
}

// ============ SEARCH OPERATIONS ============

async function searchBooks(event) {
    event.preventDefault();
    
    const title = document.getElementById('searchTitle').value.toLowerCase();
    const author = document.getElementById('searchAuthor').value.toLowerCase();
    const category = document.getElementById('searchCategory').value.toLowerCase();
    
    let results = mockData.books;
    
    if (title) {
        results = results.filter(b => b.title.toLowerCase().includes(title));
    }
    if (author) {
        results = results.filter(b => b.author.toLowerCase().includes(author));
    }
    if (category) {
        results = results.filter(b => b.category.toLowerCase().includes(category));
    }
    
    const tbody = document.getElementById('searchResultsBody');
    
    if (results.length === 0) {
        tbody.innerHTML = '<tr><td colspan="5" class="no-data">No books found matching your criteria</td></tr>';
        return;
    }
    
    tbody.innerHTML = results.map(book => `
        <tr>
            <td>${book.bookID}</td>
            <td><strong>${book.title}</strong></td>
            <td>${book.author}</td>
            <td><span class="badge badge-info">${book.category}</span></td>
            <td>
                <span class="availability ${book.availableCopies > 0 ? 'available' : 'unavailable'}">
                    ${book.availableCopies > 0 ? 'Available' : 'Unavailable'}
                </span>
            </td>
        </tr>
    `).join('');
    
    showToast(`Found ${results.length} book(s)`, 'success');
}

function clearSearch() {
    document.getElementById('searchTitle').value = '';
    document.getElementById('searchAuthor').value = '';
    document.getElementById('searchCategory').value = '';
    document.getElementById('searchResultsBody').innerHTML = '<tr><td colspan="5" class="no-data">Enter search criteria above</td></tr>';
}

// ============ STATISTICS ============

async function loadStatistics() {
    try {
        // Dashboard stats
        const totalBooks = mockData.books.length;
        const availableBooks = mockData.books.filter(b => b.availableCopies > 0).length;
        const borrowedBooks = mockData.books.reduce((sum, b) => sum + (b.totalCopies - b.availableCopies), 0);
        const totalUsers = mockData.users.length;
        
        document.getElementById('statTotalBooks').textContent = totalBooks;
        document.getElementById('statAvailable').textContent = availableBooks;
        document.getElementById('statBorrowed').textContent = borrowedBooks;
        document.getElementById('statUsers').textContent = totalUsers;
        
        // Most borrowed books
        const borrowedCount = {};
        mockData.books.forEach(book => {
            borrowedCount[book.bookID] = book.totalCopies - book.availableCopies;
        });
        
        const sortedBooks = Object.entries(borrowedCount)
            .sort((a, b) => b[1] - a[1])
            .slice(0, 5);
        
        const mostBorrowedBody = document.getElementById('mostBorrowedBody');
        mostBorrowedBody.innerHTML = sortedBooks.map(([bookID, count], index) => {
            const book = mockData.books.find(b => b.bookID == bookID);
            return book ? `
                <tr>
                    <td><strong>#${index + 1}</strong></td>
                    <td>${book.title}</td>
                    <td>${book.author}</td>
                    <td><span class="badge badge-success">${count}</span></td>
                </tr>
            ` : '';
        }).join('');
        
        // Most active users
        const sortedUsers = [...mockData.users]
            .sort((a, b) => b.borrowedBooks.length - a.borrowedBooks.length)
            .slice(0, 5);
        
        const mostActiveBody = document.getElementById('mostActiveBody');
        mostActiveBody.innerHTML = sortedUsers.map((user, index) => `
            <tr>
                <td><strong>#${index + 1}</strong></td>
                <td>${user.name}</td>
                <td>${user.email}</td>
                <td><span class="badge badge-primary">${user.borrowedBooks.length}</span></td>
            </tr>
        `).join('');
        
        // Category distribution
        const categoryStats = {};
        mockData.books.forEach(book => {
            if (!categoryStats[book.category]) {
                categoryStats[book.category] = { total: 0, available: 0, borrowed: 0 };
            }
            categoryStats[book.category].total++;
            if (book.availableCopies > 0) {
                categoryStats[book.category].available++;
            }
            categoryStats[book.category].borrowed += (book.totalCopies - book.availableCopies);
        });
        
        const categoryDistBody = document.getElementById('categoryDistBody');
        categoryDistBody.innerHTML = Object.entries(categoryStats).map(([category, stats]) => {
            const borrowRate = stats.total > 0 ? (stats.borrowed / (stats.total * 2) * 100).toFixed(1) : 0;
            return `
                <tr>
                    <td><strong>${category}</strong></td>
                    <td>${stats.total}</td>
                    <td>${stats.available}</td>
                    <td>${stats.borrowed}</td>
                    <td>
                        <div class="progress-bar">
                            <div class="progress-fill" style="width: ${borrowRate}%"></div>
                            <span class="progress-text">${borrowRate}%</span>
                        </div>
                    </td>
                </tr>
            `;
        }).join('');
        
    } catch (error) {
        showToast('Error loading statistics: ' + error.message, 'error');
    }
}

// ============ UTILITY FUNCTIONS ============

function updateHeaderStats() {
    const totalBooks = mockData.books.length;
    const totalUsers = mockData.users.length;
    const borrowedBooks = mockData.books.reduce((sum, b) => sum + (b.totalCopies - b.availableCopies), 0);
    
    document.getElementById('totalBooks').textContent = totalBooks;
    document.getElementById('totalUsers').textContent = totalUsers;
    document.getElementById('borrowedBooks').textContent = borrowedBooks;
}

function showToast(message, type = 'info') {
    const toast = document.getElementById('toast');
    toast.textContent = message;
    toast.className = `toast toast-${type} show`;
    
    setTimeout(() => {
        toast.classList.remove('show');
    }, 3000);
}

// ============ MOCK DATA GENERATORS ============

function generateMockBooks() {
    return [
        { bookID: 101, title: "The Great Gatsby", author: "F. Scott Fitzgerald", isbn: "978-0743273565", category: "Fiction", totalCopies: 3, availableCopies: 2 },
        { bookID: 102, title: "To Kill a Mockingbird", author: "Harper Lee", isbn: "978-0061120084", category: "Fiction", totalCopies: 2, availableCopies: 1 },
        { bookID: 103, title: "1984", author: "George Orwell", isbn: "978-0451524935", category: "Dystopian", totalCopies: 4, availableCopies: 2 },
        { bookID: 104, title: "Pride and Prejudice", author: "Jane Austen", isbn: "978-0141439518", category: "Romance", totalCopies: 2, availableCopies: 2 },
        { bookID: 105, title: "The Catcher in the Rye", author: "J.D. Salinger", isbn: "978-0316769174", category: "Fiction", totalCopies: 3, availableCopies: 3 },
        { bookID: 106, title: "Animal Farm", author: "George Orwell", isbn: "978-0451526342", category: "Political Fiction", totalCopies: 2, availableCopies: 1 },
        { bookID: 107, title: "Lord of the Flies", author: "William Golding", isbn: "978-0399501487", category: "Adventure", totalCopies: 2, availableCopies: 2 },
        { bookID: 108, title: "Brave New World", author: "Aldous Huxley", isbn: "978-0060850524", category: "Dystopian", totalCopies: 3, availableCopies: 3 },
        { bookID: 109, title: "The Hobbit", author: "J.R.R. Tolkien", isbn: "978-0547928227", category: "Fantasy", totalCopies: 5, availableCopies: 4 },
        { bookID: 110, title: "Fahrenheit 451", author: "Ray Bradbury", isbn: "978-1451673319", category: "Science Fiction", totalCopies: 2, availableCopies: 2 }
    ];
}

function generateMockUsers() {
    return [
        { userID: 1001, name: "Alice Johnson", email: "alice@library.com", role: "librarian", borrowedBooks: [101, 103] },
        { userID: 1002, name: "Bob Smith", email: "bob@library.com", role: "member", borrowedBooks: [102] },
        { userID: 1003, name: "Carol Davis", email: "carol@library.com", role: "member", borrowedBooks: [109] },
        { userID: 1004, name: "David Wilson", email: "david@library.com", role: "admin", borrowedBooks: [] },
        { userID: 1005, name: "Emma Brown", email: "emma@library.com", role: "member", borrowedBooks: [106] }
    ];
}

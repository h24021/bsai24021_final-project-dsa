
const API_BASE = window.ENV && window.ENV.API_BASE ? window.ENV.API_BASE : 'http://localhost:8080/api/v1';
let transactions = [];
let cache = {
    books: [],
    users: []
};

document.addEventListener('DOMContentLoaded', () => {
    initializeTabs();
    loadInitialData();
});

function initializeTabs() {
    const tabBtns = document.querySelectorAll('.tab-btn');
    const tabContents = document.querySelectorAll('.tab-content');
    
    tabBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            tabBtns.forEach(b => b.classList.remove('active'));
            tabContents.forEach(c => c.classList.remove('active'));
            
            btn.classList.add('active');
            const tabId = btn.getAttribute('data-tab') + '-tab';
            document.getElementById(tabId).classList.add('active');
            
            loadTabData(btn.getAttribute('data-tab'));
        });
    });
}

function loadInitialData() {
    loadBooks();
    loadUsers();
    updateHeaderStats();
}

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


async function loadBooks() {
    try {
        showToast('Loading books...', 'info');
        const response = await fetch(`${API_BASE}/books`);
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}: ${response.statusText}`);
        }
        
        const data = await response.json();
        const books = data.data || [];
        cache.books = books;
        
        const tbody = document.getElementById('booksTableBody');
        if (books.length === 0) {
            tbody.innerHTML = '<tr><td colspan="6" class="no-data">No books found</td></tr>';
            return;
        }
        
        tbody.innerHTML = books.map(book => `
            <tr>
                <td>${book.id}</td>
                <td><strong>${book.title}</strong></td>
                <td>${book.author}</td>
                <td><span class="badge badge-info">${book.category}</span></td>
                <td>
                    <span class="availability ${book.availableCopies > 0 ? 'available' : 'unavailable'}">
                        ${book.availableCopies} / ${book.copies}
                    </span>
                </td>
                <td>
                    <button class="btn btn-sm btn-info" onclick="viewBook(${book.id})">View</button>
                    <button class="btn btn-sm btn-warning" onclick="editBook(${book.id})">Edit</button>
                    <button class="btn btn-sm btn-danger" onclick="deleteBook(${book.id})">Delete</button>
                </td>
            </tr>
        `).join('');
        
        updateHeaderStats();
        showToast(`Loaded ${books.length} books`, 'success');
    } catch (error) {
        console.error('Error loading books:', error);
        showToast('Error loading books: ' + error.message, 'error');
        document.getElementById('booksTableBody').innerHTML = 
            `<tr><td colspan="6" class="no-data">Error: ${error.message}</td></tr>`;
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
        copies: parseInt(document.getElementById('bookCopies').value) || 1
    };
    
    try {
        showToast('Adding book...', 'info');
        const response = await fetch(`${API_BASE}/books`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(bookData)
        });
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        const result = await response.json();
        
        hideAddBookForm();
        loadBooks();
        showToast(`Book "${bookData.title}" added successfully!`, 'success');
    } catch (error) {
        console.error('Error adding book:', error);
        showToast('Error adding book: ' + error.message, 'error');
    }
}

function viewBook(bookID) {
    const book = cache.books.find(b => b.id === bookID);
    if (book) {
        alert(`Book Details:\n\nID: ${book.id}\nTitle: ${book.title}\nAuthor: ${book.author}\nISBN: ${book.isbn}\nCategory: ${book.category}\nAvailable: ${book.availableCopies}/${book.copies}`);
    }
}

function editBook(bookID) {
    showToast('Edit functionality - Connect to PUT /books/:id endpoint', 'info');
}

async function deleteBook(bookID) {
    if (!confirm('Are you sure you want to delete this book?')) return;
    
    try {
        showToast('Deleting book...', 'info');
        const response = await fetch(`${API_BASE}/books/${bookID}`, {
            method: 'DELETE'
        });
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        loadBooks();
        showToast('Book deleted successfully', 'success');
    } catch (error) {
        console.error('Error deleting book:', error);
        showToast('Error deleting book: ' + error.message, 'error');
    }
}


async function loadUsers() {
    try {
        showToast('Loading users...', 'info');
        const response = await fetch(`${API_BASE}/users`);
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}: ${response.statusText}`);
        }
        
        const result = await response.json();
        const users = result.data || [];
        cache.users = users;
        
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
                <td>${user.borrowedBooks ? user.borrowedBooks.length : 0} books</td>
                <td>
                    <button class="btn btn-sm btn-info" onclick="viewUserBorrowedBooks(${user.userID})">Borrowed</button>
                    <button class="btn btn-sm btn-warning" onclick="editUser(${user.userID})">Edit</button>
                    <button class="btn btn-sm btn-danger" onclick="deleteUser(${user.userID})">Delete</button>
                </td>
            </tr>
        `).join('');
        
        updateHeaderStats();
        showToast(`Loaded ${users.length} users`, 'success');
    } catch (error) {
        console.error('Error loading users:', error);
        showToast('Error loading users: ' + error.message, 'error');
        document.getElementById('usersTableBody').innerHTML = 
            `<tr><td colspan="6" class="no-data">Error: ${error.message}</td></tr>`;
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
        showToast('Adding user...', 'info');
        const response = await fetch(`${API_BASE}/users`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(userData)
        });
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        hideAddUserForm();
        loadUsers();
        showToast(`User "${userData.name}" added successfully!`, 'success');
    } catch (error) {
        console.error('Error adding user:', error);
        showToast('Error adding user: ' + error.message, 'error');
    }
}

async function viewUserBorrowedBooks(userID) {
    try {
        const response = await fetch(`${API_BASE}/users/${userID}/borrowed`);
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }
        
        const result = await response.json();
        const borrowedBooks = result.data || [];
        const user = cache.users.find(u => u.userID === userID);
        const userName = user ? user.name : `User ${userID}`;
        
        if (borrowedBooks.length === 0) {
            alert(`${userName} has no borrowed books.`);
        } else {
            const booksList = borrowedBooks.map(book => 
                `- ${book.title} by ${book.author}`
            ).join('\n');
            alert(`${userName}'s Borrowed Books:\n\n${booksList}`);
        }
    } catch (error) {
        console.error('Error fetching borrowed books:', error);
        showToast('Error loading borrowed books: ' + error.message, 'error');
    }
}

function editUser(userID) {
    showToast('Edit functionality - Connect to PUT /users/:id endpoint', 'info');
}

async function deleteUser(userID) {
    if (!confirm('Are you sure you want to delete this user?')) return;
    
    try {
        showToast('Deleting user...', 'info');
        const response = await fetch(`${API_BASE}/users/${userID}`, {
            method: 'DELETE'
        });
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        loadUsers();
        showToast('User deleted successfully', 'success');
    } catch (error) {
        console.error('Error deleting user:', error);
        showToast('Error deleting user: ' + error.message, 'error');
    }
}


async function borrowBook(event) {
    event.preventDefault();
    
    const userID = parseInt(document.getElementById('borrowUserID').value);
    const bookID = parseInt(document.getElementById('borrowBookID').value);
    
    try {
        showToast('Processing borrow...', 'info');
        const response = await fetch(`${API_BASE}/borrow`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ userID, bookID })
        });
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        const result = await response.json();
        
        transactions.unshift({
            type: 'borrow',
            userID,
            userName: result.userName || `User ${userID}`,
            bookID,
            bookTitle: result.bookTitle || `Book ${bookID}`,
            timestamp: new Date().toLocaleString()
        });
        
        updateTransactionHistory();
        loadBooks();
        updateHeaderStats();
        
        document.getElementById('borrowUserID').value = '';
        document.getElementById('borrowBookID').value = '';
        
        showToast(result.message || 'Book borrowed successfully!', 'success');
    } catch (error) {
        console.error('Error borrowing book:', error);
        showToast('Error borrowing book: ' + error.message, 'error');
    }
}

async function returnBook(event) {
    event.preventDefault();
    
    const userID = parseInt(document.getElementById('returnUserID').value);
    const bookID = parseInt(document.getElementById('returnBookID').value);
    
    try {
        showToast('Processing return...', 'info');
        const response = await fetch(`${API_BASE}/return`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ userID, bookID })
        });
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        const result = await response.json();
        
        transactions.unshift({
            type: 'return',
            userID,
            userName: result.userName || `User ${userID}`,
            bookID,
            bookTitle: result.bookTitle || `Book ${bookID}`,
            timestamp: new Date().toLocaleString()
        });
        
        updateTransactionHistory();
        loadBooks();
        updateHeaderStats();
        
        document.getElementById('returnUserID').value = '';
        document.getElementById('returnBookID').value = '';
        
        showToast(result.message || 'Book returned successfully!', 'success');
    } catch (error) {
        console.error('Error returning book:', error);
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


async function searchBooks(event) {
    event.preventDefault();
    
    const title = document.getElementById('searchTitle').value;
    const author = document.getElementById('searchAuthor').value;
    const category = document.getElementById('searchCategory').value;
    
    if (!title && !author && !category) {
        showToast('Please enter at least one search criterion', 'error');
        return;
    }
    
    try {
        showToast('Searching...', 'info');
        let queryParams = [];
        if (title) queryParams.push(`title=${encodeURIComponent(title)}`);
        if (author) queryParams.push(`author=${encodeURIComponent(author)}`);
        if (category) queryParams.push(`category=${encodeURIComponent(category)}`);
        
        const response = await fetch(`${API_BASE}/books/search?${queryParams.join('&')}`);
        
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message || `HTTP ${response.status}`);
        }
        
        const result = await response.json();
        const results = result.data || [];
        
        const tbody = document.getElementById('searchResultsBody');
        
        if (results.length === 0) {
            tbody.innerHTML = '<tr><td colspan="5" class="no-data">No books found matching your criteria</td></tr>';
            showToast('No books found', 'info');
            return;
        }
        
        tbody.innerHTML = results.map(book => `
            <tr>
                <td>${book.id}</td>
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
    } catch (error) {
        console.error('Error searching books:', error);
        showToast('Error searching books: ' + error.message, 'error');
    }
}

function clearSearch() {
    document.getElementById('searchTitle').value = '';
    document.getElementById('searchAuthor').value = '';
    document.getElementById('searchCategory').value = '';
    document.getElementById('searchResultsBody').innerHTML = '<tr><td colspan="5" class="no-data">Enter search criteria above</td></tr>';
}


async function loadStatistics() {
    try {
        showToast('Loading statistics...', 'info');
        const response = await fetch(`${API_BASE}/dashboard`);
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }
        
        const result = await response.json();
        const dashboardData = result.data || {};
        const overview = dashboardData.overview || {};
        const categoryDist = dashboardData.categoryDistribution || [];
        
        document.getElementById('statTotalBooks').textContent = overview.totalBooks || 0;
        document.getElementById('statAvailable').textContent = overview.availableBooks || 0;
        document.getElementById('statBorrowed').textContent = overview.borrowedBooks || 0;
        document.getElementById('statUsers').textContent = overview.totalUsers || 0;
        
        const categoryDistBody = document.getElementById('categoryDistBody');
        if (categoryDist.length > 0) {
            categoryDistBody.innerHTML = categoryDist.map(cat => {
                const borrowRate = cat.count > 0 ? (cat.count / overview.totalBooks * 100).toFixed(1) : 0;
                return `
                    <tr>
                        <td><strong>${cat.category}</strong></td>
                        <td>${cat.count}</td>
                        <td>-</td>
                        <td>-</td>
                        <td>
                            <div class="progress-bar">
                                <div class="progress-fill" style="width: ${borrowRate}%"></div>
                                <span class="progress-text">${borrowRate}%</span>
                            </div>
                        </td>
                    </tr>
                `;
            }).join('');
        }
        
        loadMostBorrowedBooks();
        
        loadMostActiveUsers();
        
        showToast('Statistics loaded', 'success');
    } catch (error) {
        console.error('Error loading statistics:', error);
        showToast('Error loading statistics: ' + error.message, 'error');
    }
}

async function loadMostBorrowedBooks() {
    try {
        const response = await fetch(`${API_BASE}/statistics/most-borrowed?limit=5`);
        if (response.ok) {
            const result = await response.json();
            const books = result.data || [];
            
            const mostBorrowedBody = document.getElementById('mostBorrowedBody');
            mostBorrowedBody.innerHTML = books.map((book, index) => `
                <tr>
                    <td><strong>#${index + 1}</strong></td>
                    <td>${book.title}</td>
                    <td>${book.author}</td>
                    <td><span class="badge badge-success">${book.timesCirculated || 0}</span></td>
                </tr>
            `).join('');
        } else {
            const sortedBooks = [...cache.books]
                .sort((a, b) => (b.copies - b.availableCopies) - (a.copies - a.availableCopies))
                .slice(0, 5);
            
            const mostBorrowedBody = document.getElementById('mostBorrowedBody');
            mostBorrowedBody.innerHTML = sortedBooks.map((book, index) => `
                <tr>
                    <td><strong>#${index + 1}</strong></td>
                    <td>${book.title}</td>
                    <td>${book.author}</td>
                    <td><span class="badge badge-success">${book.copies - book.availableCopies}</span></td>
                </tr>
            `).join('');
        }
    } catch (error) {
        console.error('Error loading most borrowed books:', error);
    }
}

async function loadMostActiveUsers() {
    try {
        const response = await fetch(`${API_BASE}/statistics/most-active?limit=5`);
        if (response.ok) {
            const result = await response.json();
            const users = result.data || [];
            
            const mostActiveBody = document.getElementById('mostActiveBody');
            mostActiveBody.innerHTML = users.map((user, index) => `
                <tr>
                    <td><strong>#${index + 1}</strong></td>
                    <td>${user.name}</td>
                    <td>${user.email}</td>
                    <td><span class="badge badge-primary">${user.booksBorrowed || 0}</span></td>
                </tr>
            `).join('');
        } else {
            const sortedUsers = [...cache.users]
                .sort((a, b) => (b.borrowedBooks?.length || 0) - (a.borrowedBooks?.length || 0))
                .slice(0, 5);
            
            const mostActiveBody = document.getElementById('mostActiveBody');
            mostActiveBody.innerHTML = sortedUsers.map((user, index) => `
                <tr>
                    <td><strong>#${index + 1}</strong></td>
                    <td>${user.name}</td>
                    <td>${user.email}</td>
                    <td><span class="badge badge-primary">${user.borrowedBooks?.length || 0}</span></td>
                </tr>
            `).join('');
        }
    } catch (error) {
        console.error('Error loading most active users:', error);
    }
}


function updateHeaderStats() {
    const totalBooks = cache.books.length;
    const totalUsers = cache.users.length;
    const borrowedBooks = cache.books.reduce((sum, b) => sum + (b.copies - b.availableCopies), 0);
    
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



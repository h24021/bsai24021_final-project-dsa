// Library Management System - Main Application
const API_URL = window.ENV.API_BASE;

// Tab switching
document.querySelectorAll('.main-tab').forEach(tab => {
    tab.addEventListener('click', () => {
        const section = tab.dataset.section;
        
        // Update active tab
        document.querySelectorAll('.main-tab').forEach(t => t.classList.remove('active'));
        tab.classList.add('active');
        
        // Update active section
        document.querySelectorAll('.content-section').forEach(s => s.classList.remove('active'));
        document.getElementById(`${section}-section`).classList.add('active');
        
        // Load data for the section
        if (section === 'users') loadUsers();
        if (section === 'borrow') loadBorrowHistory();
    });
});

// User Management
async function loadUsers() {
    try {
        const response = await fetch(`${API_URL}/users`);
        const data = await response.json();
        
        if (data.status === 'success') {
            displayUsers(data.data);
        }
    } catch (error) {
        console.error('Error loading users:', error);
    }
}

function displayUsers(users) {
    const container = document.getElementById('usersList');
    container.innerHTML = '<h3>All Users</h3>';
    
    users.forEach(user => {
        const userCard = document.createElement('div');
        userCard.className = 'item-card';
        userCard.innerHTML = `
            <div class="item-info">
                <h4>${user.name}</h4>
                <p>ID: ${user.id} | Email: ${user.email || 'N/A'} | Role: ${user.role || 'Member'}</p>
            </div>
        `;
        container.appendChild(userCard);
    });
}

async function addUser() {
    const name = document.getElementById('userName').value;
    const email = document.getElementById('userEmail').value;
    const role = document.getElementById('userRole').value;
    
    if (!name) {
        alert('Please enter a name');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/users`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name, email, role })
        });
        
        const data = await response.json();
        
        if (data.status === 'success') {
            alert('User added successfully!');
            document.getElementById('userName').value = '';
            document.getElementById('userEmail').value = '';
            loadUsers();
        } else {
            alert('Error: ' + data.message);
        }
    } catch (error) {
        alert('Error adding user: ' + error.message);
    }
}

// Borrow/Return
async function borrowBook() {
    const userId = document.getElementById('borrowUserId').value;
    const bookId = document.getElementById('borrowBookId').value;
    
    if (!userId || !bookId) {
        alert('Please enter both User ID and Book ID');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/borrow`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ userId: parseInt(userId), bookId: parseInt(bookId) })
        });
        
        const data = await response.json();
        
        if (data.status === 'success') {
            alert('Book borrowed successfully!');
            document.getElementById('borrowUserId').value = '';
            document.getElementById('borrowBookId').value = '';
            loadBorrowHistory();
        } else {
            alert('Error: ' + data.message);
        }
    } catch (error) {
        alert('Error borrowing book: ' + error.message);
    }
}

async function returnBook() {
    const userId = document.getElementById('returnUserId').value;
    const bookId = document.getElementById('returnBookId').value;
    
    if (!userId || !bookId) {
        alert('Please enter both User ID and Book ID');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/return`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ userId: parseInt(userId), bookId: parseInt(bookId) })
        });
        
        const data = await response.json();
        
        if (data.status === 'success') {
            alert('Book returned successfully!');
            document.getElementById('returnUserId').value = '';
            document.getElementById('returnBookId').value = '';
            loadBorrowHistory();
        } else {
            alert('Error: ' + data.message);
        }
    } catch (error) {
        alert('Error returning book: ' + error.message);
    }
}

async function loadBorrowHistory() {
    const container = document.getElementById('borrowHistory');
    container.innerHTML = '<h3>Recent Borrow History</h3><p>Use the forms above to borrow or return books</p>';
}

// Search
async function searchBooks() {
    const query = document.getElementById('searchQuery').value;
    
    if (!query) {
        alert('Please enter a search query');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/books/search?query=${encodeURIComponent(query)}`);
        const data = await response.json();
        
        if (data.status === 'success') {
            displaySearchResults(data.data);
        }
    } catch (error) {
        console.error('Error searching:', error);
    }
}

function displaySearchResults(books) {
    const container = document.getElementById('searchResults');
    container.innerHTML = '';
    
    if (books.length === 0) {
        container.innerHTML = '<p class="empty-state">No books found</p>';
        return;
    }
    
    books.forEach(book => {
        const bookCard = document.createElement('div');
        bookCard.className = 'book-item';
        
        const cover = document.createElement('div');
        cover.className = 'book-cover';
        
        if (book.coverImage) {
            const img = document.createElement('img');
            img.src = book.coverImage;
            img.alt = book.title;
            img.onerror = function() {
                this.style.display = 'none';
                const placeholder = document.createElement('div');
                placeholder.className = 'book-cover-placeholder';
                cover.appendChild(placeholder);
            };
            cover.appendChild(img);
        } else {
            const placeholder = document.createElement('div');
            placeholder.className = 'book-cover-placeholder';
            cover.appendChild(placeholder);
        }
        
        const title = document.createElement('div');
        title.className = 'book-title';
        title.textContent = book.title;
        
        const author = document.createElement('div');
        author.className = 'book-author';
        author.textContent = book.author;
        
        const info = document.createElement('div');
        info.className = 'book-info';
        info.textContent = `ID: ${book.id} | Available: ${book.availableCopies}/${book.copies}`;
        
        bookCard.appendChild(cover);
        bookCard.appendChild(title);
        bookCard.appendChild(author);
        bookCard.appendChild(info);
        
        // Add download link if available
        if (book.downloadLinks && book.downloadLinks.length > 0) {
            bookCard.style.cursor = 'pointer';
            bookCard.addEventListener('click', () => {
                window.open(book.downloadLinks[0], '_blank');
            });
        }
        
        container.appendChild(bookCard);
    });
}

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    console.log('Library Management System loaded');
});

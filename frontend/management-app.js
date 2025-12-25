// Library Management System - Main Application
const API_URL = window.ENV.API_BASE;

let allBooks = []; // Store all books for filtering

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
        if (section === 'books') loadAllBooks();
        if (section === 'users') loadUsers();
        if (section === 'borrow') loadBorrowHistory();
        if (section === 'search') loadAllBooksForSearch();
    });
});

// Books Management - Show all books with IDs
async function loadAllBooks() {
    try {
        const response = await fetch(`${API_URL}/books`);
        const data = await response.json();
        
        if (data.status === 'success') {
            allBooks = data.data;
            displayAllBooks(allBooks);
        }
    } catch (error) {
        console.error('Error loading books:', error);
        alert('Error loading books: ' + error.message);
    }
}

function filterBooks(type) {
    // Update active filter button
    document.querySelectorAll('.filter-btn').forEach(btn => btn.classList.remove('active'));
    event.target.classList.add('active');
    
    if (type === 'all') {
        displayAllBooks(allBooks);
    } else {
        const filtered = allBooks.filter(book => book.type === type);
        displayAllBooks(filtered);
    }
}

function displayAllBooks(books) {
    const container = document.getElementById('allBooksGrid');
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
        
        const bookInfo = document.createElement('div');
        bookInfo.className = 'book-details';
        
        const title = document.createElement('div');
        title.className = 'book-title';
        title.textContent = book.title;
        
        const author = document.createElement('div');
        author.className = 'book-author';
        author.textContent = book.author;
        
        const bookId = document.createElement('div');
        bookId.className = 'book-id-badge';
        bookId.innerHTML = `<strong>Book ID: ${book.id}</strong>`;
        
        const info = document.createElement('div');
        info.className = 'book-info';
        info.innerHTML = `Available: ${book.availableCopies}/${book.copies} | ${book.type || 'Book'}`;
        
        bookInfo.appendChild(title);
        bookInfo.appendChild(author);
        bookInfo.appendChild(bookId);
        bookInfo.appendChild(info);
        
        bookCard.appendChild(cover);
        bookCard.appendChild(bookInfo);
        
        // Add download link if available
        if (book.downloadLinks && book.downloadLinks.length > 0) {
            const downloadBtn = document.createElement('button');
            downloadBtn.className = 'btn-download';
            downloadBtn.textContent = 'Download/Read';
            downloadBtn.onclick = () => window.open(book.downloadLinks[0], '_blank');
            bookCard.appendChild(downloadBtn);
        }
        
        container.appendChild(bookCard);
    });
}

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
        alert('Error loading users: ' + error.message);
    }
}

function displayUsers(users) {
    const container = document.getElementById('usersList');
    container.innerHTML = '<h3>All Users</h3>';
    
    if (users.length === 0) {
        container.innerHTML += '<p class="empty-state">No users yet. Add your first user above!</p>';
        return;
    }
    
    users.forEach(user => {
        const userId = user.userID || user.id;  // Handle both formats
        const userCard = document.createElement('div');
        userCard.className = 'item-card';
        userCard.innerHTML = `
            <div class="item-info">
                <h4>${user.name}</h4>
                <p><strong>User ID: ${userId}</strong> | Email: ${user.email || 'N/A'} | Role: ${user.role || 'Member'}</p>
                <p class="id-highlight">Use ID ${userId} for borrowing/returning books</p>
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
            const userId = data.data.userID || data.data.id;  // Handle both formats
            alert(`✅ User added successfully!\n\nName: ${name}\nUser ID: ${userId}\n\nYou can now use this User ID to borrow books.`);
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
            alert('✅ Book borrowed successfully!\n\nUser ID: ' + userId + '\nBook ID: ' + bookId);
            document.getElementById('borrowUserId').value = '';
            document.getElementById('borrowBookId').value = '';
            loadBorrowHistory();
        } else {
            alert('❌ Error: ' + (data.message || 'Failed to borrow book'));
        }
    } catch (error) {
        alert('❌ Error borrowing book: ' + error.message);
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
            alert('✅ Book returned successfully!\n\nUser ID: ' + userId + '\nBook ID: ' + bookId);
            document.getElementById('returnUserId').value = '';
            document.getElementById('returnBookId').value = '';
            loadBorrowHistory();
        } else {
            alert('❌ Error: ' + (data.message || 'Failed to return book'));
        }
    } catch (error) {
        alert('❌ Error returning book: ' + error.message);
    }
}

async function loadBorrowHistory() {
    const container = document.getElementById('borrowHistory');
    container.innerHTML = '<p style="color: var(--light-gray); text-align: center; padding: 2rem;">Use the forms above to borrow or return books</p>';
}

// Search
async function loadAllBooksForSearch() {
    try {
        const response = await fetch(`${API_URL}/books`);
        const data = await response.json();
        
        if (data.status === 'success') {
            displaySearchResults(data.data.slice(0, 20)); // Show first 20 books
            document.getElementById('searchQuery').placeholder = 'Search by title, author, or ISBN... (showing first 20 books)';
        }
    } catch (error) {
        console.error('Error loading books:', error);
    }
}

async function searchBooks() {
    const query = document.getElementById('searchQuery').value;
    
    if (!query) {
        alert('Please enter a search query');
        return;
    }
    
    try {
        // Search by title by default - backend expects title, author, or category parameter
        const response = await fetch(`${API_URL}/books/search?title=${encodeURIComponent(query)}`);
        const data = await response.json();
        
        if (data.status === 'success') {
            displaySearchResults(data.data);
        } else {
            alert('Search failed: ' + data.message);
        }
    } catch (error) {
        console.error('Error searching:', error);
        alert('Error searching: ' + error.message);
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
        info.innerHTML = `<strong>Book ID: ${book.id}</strong><br>Available: ${book.availableCopies}/${book.copies}`;
        
        bookCard.appendChild(cover);
        bookCard.appendChild(title);
        bookCard.appendChild(author);
        bookCard.appendChild(info);
        
        // Add download link if available
        if (book.downloadLinks && book.downloadLinks.length > 0) {
            const downloadBtn = document.createElement('button');
            downloadBtn.className = 'btn-download';
            downloadBtn.textContent = 'Download/Read';
            downloadBtn.onclick = () => window.open(book.downloadLinks[0], '_blank');
            bookCard.appendChild(downloadBtn);
        }
        
        container.appendChild(bookCard);
    });
}

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    console.log('Library Management System loaded');
    // Load books on page load (Books tab is active by default)
    loadAllBooks();
});

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
            body: JSON.stringify({ userID: parseInt(userId), bookID: parseInt(bookId) })
        });
        
        const data = await response.json();
        
        if (data.message && data.message.includes('successfully')) {
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
            body: JSON.stringify({ userID: parseInt(userId), bookID: parseInt(bookId) })
        });
        
        const data = await response.json();
        
        if (data.message && data.message.includes('successfully')) {
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
    container.innerHTML = '<p style="color: var(--light-gray); text-align: center; padding: 2rem;">Loading borrowed books...</p>';
    
    try {
        // Get all users
        const usersResponse = await fetch(`${API_URL}/users`);
        const usersData = await usersResponse.json();
        
        if (usersData.status !== 'success') {
            container.innerHTML = '<p style="color: var(--light-gray); text-align: center; padding: 2rem;">No borrow history available</p>';
            return;
        }
        
        const users = usersData.data;
        const allBorrowedBooks = [];
        
        // For each user, get their borrowed books
        for (const user of users) {
            const userId = user.userID || user.id;
            try {
                const borrowedResponse = await fetch(`${API_URL}/users/${userId}/borrowed`);
                const borrowedData = await borrowedResponse.json();
                
                if (borrowedData.status === 'success' && borrowedData.data && borrowedData.data.length > 0) {
                    borrowedData.data.forEach(book => {
                        allBorrowedBooks.push({
                            userId: userId,
                            userName: user.name,
                            userEmail: user.email,
                            bookId: book.bookID,
                            bookTitle: book.title,
                            bookAuthor: book.author
                        });
                    });
                }
            } catch (error) {
                console.error(`Error fetching borrowed books for user ${userId}:`, error);
            }
        }
        
        // Display the borrowed books
        if (allBorrowedBooks.length === 0) {
            container.innerHTML = '<p style="color: var(--light-gray); text-align: center; padding: 2rem;">No books currently borrowed</p>';
            return;
        }
        
        container.innerHTML = `
            <h3 style="color: var(--white); margin-bottom: 1.5rem; font-size: 1.2rem;">Currently Borrowed Books (${allBorrowedBooks.length})</h3>
            <div class="borrow-history-list">
                ${allBorrowedBooks.map(item => `
                    <div class="borrow-item" style="background: rgba(255,255,255,0.05); padding: 1rem; margin-bottom: 0.8rem; border-radius: 8px; border-left: 4px solid var(--maroon);">
                        <div style="display: flex; justify-content: space-between; align-items: start;">
                            <div style="flex: 1;">
                                <div style="color: var(--white); font-weight: 600; font-size: 1.1rem; margin-bottom: 0.5rem;">
                                    ${item.bookTitle}
                                </div>
                                <div style="color: var(--light-gray); font-size: 0.9rem; margin-bottom: 0.3rem;">
                                    by ${item.bookAuthor}
                                </div>
                                <div style="color: var(--light-gray); font-size: 0.85rem;">
                                    <span style="background: var(--maroon); color: white; padding: 2px 8px; border-radius: 4px; margin-right: 0.5rem;">Book ID: ${item.bookId}</span>
                                    <span>Borrowed by: ${item.userName} (User ID: ${item.userId})</span>
                                </div>
                            </div>
                            <button onclick="quickReturn(${item.userId}, ${item.bookId})" 
                                    class="btn-success" 
                                    style="margin-left: 1rem; white-space: nowrap; padding: 0.5rem 1rem; font-size: 0.9rem;">
                                Return Book
                            </button>
                        </div>
                    </div>
                `).join('')}
            </div>
        `;
        
    } catch (error) {
        console.error('Error loading borrow history:', error);
        container.innerHTML = '<p style="color: var(--maroon); text-align: center; padding: 2rem;">Error loading borrow history</p>';
    }
}

// Quick return function for the borrow history list
async function quickReturn(userId, bookId) {
    if (!confirm(`Return Book ID ${bookId} for User ID ${userId}?`)) {
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/return`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ userID: parseInt(userId), bookID: parseInt(bookId) })
        });
        
        const data = await response.json();
        
        if (data.message && data.message.includes('successfully')) {
            alert('✅ Book returned successfully!');
            loadBorrowHistory(); // Reload the list
        } else {
            alert('❌ Error: ' + (data.message || 'Failed to return book'));
        }
    } catch (error) {
        alert('❌ Error returning book: ' + error.message);
    }
}

// Search
async function loadAllBooksForSearch() {
    try {
        const response = await fetch(`${API_URL}/books`);
        const data = await response.json();
        
        if (data.status === 'success') {
            displaySearchResults(data.data.slice(0, 20)); // Show first 20 books
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
        // Search by title first
        const titleResponse = await fetch(`${API_URL}/books/search?title=${encodeURIComponent(query)}`);
        const titleData = await titleResponse.json();
        
        // Search by author
        const authorResponse = await fetch(`${API_URL}/books/search?author=${encodeURIComponent(query)}`);
        const authorData = await authorResponse.json();
        
        // Combine results and remove duplicates
        let allResults = [];
        if (titleData.status === 'success' && titleData.data) {
            allResults = [...titleData.data];
        }
        if (authorData.status === 'success' && authorData.data) {
            authorData.data.forEach(book => {
                // Only add if not already in results (check both id and bookID)
                const bookId = book.id || book.bookID;
                if (!allResults.find(b => (b.id || b.bookID) === bookId)) {
                    allResults.push(book);
                }
            });
        }
        
        if (allResults.length > 0) {
            displaySearchResults(allResults);
        } else {
            displaySearchResults([]);
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

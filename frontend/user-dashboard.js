const API_URL = window.ENV.API_BASE;

let allBooks = [];
let currentUserID = null;

document.addEventListener('DOMContentLoaded', () => {
    checkAuthentication();
    setupLogout();
});

function checkAuthentication() {
    const token = localStorage.getItem('authToken');
    const role = localStorage.getItem('userRole');
    const userName = localStorage.getItem('userName');
    const userID = localStorage.getItem('userID');
    
    if (!token) {
        window.location.href = 'login.html';
        return;
    }
    
    if (role === 'ADMIN') {
        window.location.href = 'index.html';
        return;
    }
    
    currentUserID = parseInt(userID);
    
    const welcomeMessage = document.getElementById('welcomeMessage');
    if (welcomeMessage && userName) {
        welcomeMessage.textContent = `Welcome, ${userName}`;
    }
    
    loadProfileInfo();
    
    verifyToken(token);
    
    loadAllBooks();
}

async function verifyToken(token) {
    try {
        const response = await fetch(`${API_URL}/auth/me`, {
            method: 'GET',
            headers: {
                'Authorization': `Bearer ${token}`
            }
        });
        
        if (!response.ok) {
            clearAuthData();
            window.location.href = 'login.html';
        }
    } catch (error) {
        console.error('Token verification error:', error);
        clearAuthData();
        window.location.href = 'login.html';
    }
}

function setupLogout() {
    const logoutBtn = document.getElementById('logoutBtn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', async () => {
            const token = localStorage.getItem('authToken');
            
            if (token) {
                try {
                    await fetch(`${API_URL}/auth/logout`, {
                        method: 'POST',
                        headers: {
                            'Authorization': `Bearer ${token}`
                        }
                    });
                } catch (error) {
                    console.error('Logout error:', error);
                }
            }
            
            clearAuthData();
            window.location.href = 'login.html';
        });
    }
}

function clearAuthData() {
    localStorage.removeItem('authToken');
    localStorage.removeItem('userRole');
    localStorage.removeItem('userID');
    localStorage.removeItem('username');
    localStorage.removeItem('userEmail');
    localStorage.removeItem('userName');
}

function loadProfileInfo() {
    document.getElementById('profileName').textContent = localStorage.getItem('userName') || 'N/A';
    document.getElementById('profileUsername').textContent = localStorage.getItem('username') || 'N/A';
    document.getElementById('profileEmail').textContent = localStorage.getItem('userEmail') || 'N/A';
    document.getElementById('profileUserID').textContent = localStorage.getItem('userID') || 'N/A';
    
    loadUserStatistics();
}

async function loadUserStatistics() {
    try {
        console.log('Loading statistics for user:', currentUserID);
        const response = await fetch(`${API_URL}/users/${currentUserID}/borrowed`);
        console.log('Response status:', response.status);
        
        if (response.ok) {
            const data = await response.json();
            console.log('Borrowed books data:', data);
            
            if (data.status === 'success') {
                const borrowedBooks = data.data && Array.isArray(data.data) ? data.data : [];
                
                const currentlyBorrowed = borrowedBooks.length;
                console.log('Currently borrowed count:', currentlyBorrowed);
                
                const currentlyBorrowedElement = document.getElementById('currentlyBorrowed');
                if (currentlyBorrowedElement) {
                    currentlyBorrowedElement.textContent = currentlyBorrowed;
                }
            }
        } else {
            console.error('Failed to fetch borrowed books:', response.status);
        }
    } catch (error) {
        console.error('Error loading statistics:', error);
    }
}

document.querySelectorAll('.main-tab').forEach(tab => {
    tab.addEventListener('click', () => {
        const section = tab.dataset.section;
        
        document.querySelectorAll('.main-tab').forEach(t => t.classList.remove('active'));
        tab.classList.add('active');
        
        document.querySelectorAll('.content-section').forEach(s => s.classList.remove('active'));
        document.getElementById(`${section}-section`).classList.add('active');
        
        if (section === 'books') loadAllBooks();
        if (section === 'myborrowed') loadMyBorrowedBooks();
        if (section === 'search') loadAllBooksForSearch();
        if (section === 'profile') {
            loadProfileInfo();
        }
    });
});

async function loadAllBooks() {
    try {
        const response = await fetch(`${API_URL}/books`);
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        
        if (data.status === 'success' && data.data) {
            allBooks = Array.isArray(data.data) ? data.data : JSON.parse(data.data);
            displayAllBooks(allBooks);
        }
    } catch (error) {
        console.error('Error loading books:', error);
        alert('Error loading books: ' + error.message);
    }
}

function displayAllBooks(books) {
    const grid = document.getElementById('allBooksGrid');
    if (!grid) return;
    
    if (!books || books.length === 0) {
        grid.innerHTML = '<div class="empty-state"><h3>No books available</h3></div>';
        return;
    }
    
    grid.innerHTML = books.map(book => {
        const coverImg = book.coverImage || book.cover_image || '';
        const bookId = book.id || book.bookID;
        const isAvailable = book.availableCopies > 0;
        const genre = book.type || book.category || book.genre || '';
        
        return `
        <div class="book-card ${isAvailable ? '' : 'unavailable'}">
            <div class="book-cover-container">
                ${coverImg ? 
                    `<img src="${coverImg}" 
                         alt="${book.title}" 
                         class="book-cover"
                         onerror="this.onerror=null; this.src='data:image/svg+xml,%3Csvg xmlns=\\'http://www.w3.org/2000/svg\\' width=\\'150\\' height=\\'200\\' viewBox=\\'0 0 150 200\\'%3E%3Crect fill=\\'%23f0f0f0\\' width=\\'150\\' height=\\'200\\'/%3E%3Ctext x=\\'50%25\\' y=\\'50%25\\' dominant-baseline=\\'middle\\' text-anchor=\\'middle\\' font-family=\\'Arial, sans-serif\\' font-size=\\'14\\' fill=\\'%23999\\'%3ENo Cover%3C/text%3E%3C/svg%3E';">` 
                    : 
                    `<img src="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='150' height='200' viewBox='0 0 150 200'%3E%3Crect fill='%23f0f0f0' width='150' height='200'/%3E%3Ctext x='50%25' y='50%25' dominant-baseline='middle' text-anchor='middle' font-family='Arial, sans-serif' font-size='14' fill='%23999'%3ENo Cover%3C/text%3E%3C/svg%3E" alt="No cover" class="book-cover">`
                }
            </div>
            <div class="book-info">
                ${genre ? `<div class="book-genre">${genre}</div>` : ''}
                <h3 class="book-title">${book.title}</h3>
                <p class="book-author">by ${book.author}</p>
                <div class="book-meta">
                    <span class="book-id">ID: ${bookId}</span>
                    <span class="book-copies ${isAvailable ? 'available' : 'unavailable'}">
                        ${isAvailable ? `${book.availableCopies} available` : 'Not available'}
                    </span>
                </div>
                ${book.downloadLink ? 
                    `<a href="${book.downloadLink}" target="_blank" class="btn-download">Download</a>` 
                    : ''}
                ${isAvailable ? 
                    `<button class="btn-borrow" onclick="borrowBook(${bookId})">Borrow Book</button>` : 
                    `<button class="btn-disabled" disabled>Not Available</button>`
                }
            </div>
        </div>
        `;
    }).join('');
}

function filterBooks(genre) {
    document.querySelectorAll('.filter-btn').forEach(btn => btn.classList.remove('active'));
    event.target.classList.add('active');
    
    if (genre === 'all') {
        displayAllBooks(allBooks);
    } else {
        const filtered = allBooks.filter(book => book.genre === genre);
        displayAllBooks(filtered);
    }
}

async function loadMyBorrowedBooks() {
    try {
        const response = await fetch(`${API_URL}/users/${currentUserID}/borrowed`);
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        
        if (data.status === 'success' && data.data) {
            const borrowedBooks = Array.isArray(data.data) ? data.data : JSON.parse(data.data);
            displayMyBorrowedBooks(borrowedBooks);
        }
    } catch (error) {
        console.error('Error loading borrowed books:', error);
        const container = document.getElementById('myBorrowedList');
        if (container) {
            container.innerHTML = '<div class="empty-state"><h3>Error loading borrowed books</h3></div>';
        }
    }
}

function displayMyBorrowedBooks(borrowedBooks) {
    const container = document.getElementById('myBorrowedList');
    if (!container) return;
    
    const currentlyBorrowed = borrowedBooks.filter(b => !b.returnDate);
    
    if (currentlyBorrowed.length === 0) {
        container.innerHTML = `
            <div class="empty-state">
                <h3>📚 No books currently borrowed</h3>
                <p>Browse the available books and start reading!</p>
            </div>
        `;
        return;
    }
    
    container.innerHTML = currentlyBorrowed.map(borrow => `
        <div class="borrowed-item">
            <div class="borrowed-item-info">
                <h3>${borrow.bookTitle || `Book ID: ${borrow.bookID}`}</h3>
                <p>Book ID: ${borrow.bookID}</p>
            </div>
            <button class="btn-return" onclick="returnBook(${borrow.bookID})">Return Book</button>
        </div>
    `).join('');
}

function loadAllBooksForSearch() {
    loadAllBooks();
}

async function searchBooks() {
    const query = document.getElementById('searchInput').value.trim();
    
    if (!query) {
        alert('Please enter a search term');
        return;
    }
    
    try {
        const titleResponse = await fetch(`${API_URL}/books/search?title=${encodeURIComponent(query)}`);
        const titleData = await titleResponse.json();
        
        const authorResponse = await fetch(`${API_URL}/books/search?author=${encodeURIComponent(query)}`);
        const authorData = await authorResponse.json();
        
        let allResults = [];
        
        if (titleData.status === 'success' && titleData.data && Array.isArray(titleData.data)) {
            allResults = [...titleData.data];
        }
        
        if (authorData.status === 'success' && authorData.data && Array.isArray(authorData.data)) {
            authorData.data.forEach(book => {
                if (!allResults.find(b => b.id === book.id)) {
                    allResults.push(book);
                }
            });
        }
        
        displaySearchResults(allResults);
    } catch (error) {
        console.error('Error searching:', error);
        alert('Error searching: ' + error.message);
    }
}

function displaySearchResults(books) {
    const resultsDiv = document.getElementById('searchResults');
    if (!resultsDiv) return;
    
    if (!books || books.length === 0) {
        resultsDiv.innerHTML = '<div class="empty-state"><h3>No books found</h3><p>Try a different search term</p></div>';
        return;
    }
    
    resultsDiv.innerHTML = books.map(book => {
        const coverImg = book.coverImage || book.cover_image || '';
        const bookId = book.id || book.bookID;
        const isAvailable = book.availableCopies > 0 || book.borrowed === 'N' || book.borrowed === false;
        const genre = book.type || book.category || book.genre || '';
        
        return `
        <div class="book-card ${isAvailable ? '' : 'unavailable'}">
            <div class="book-cover-container">
                ${coverImg ? 
                    `<img src="${coverImg}" 
                         alt="${book.title}" 
                         class="book-cover"
                         onerror="this.onerror=null; this.src='data:image/svg+xml,%3Csvg xmlns=\\'http://www.w3.org/2000/svg\\' width=\\'150\\' height=\\'200\\' viewBox=\\'0 0 150 200\\'%3E%3Crect fill=\\'%23f0f0f0\\' width=\\'150\\' height=\\'200\\'/%3E%3Ctext x=\\'50%25\\' y=\\'50%25\\' dominant-baseline=\\'middle\\' text-anchor=\\'middle\\' font-family=\\'Arial, sans-serif\\' font-size=\\'14\\' fill=\\'%23999\\'%3ENo Cover%3C/text%3E%3C/svg%3E';">` 
                    : 
                    `<img src="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='150' height='200' viewBox='0 0 150 200'%3E%3Crect fill='%23f0f0f0' width='150' height='200'/%3E%3Ctext x='50%25' y='50%25' dominant-baseline='middle' text-anchor='middle' font-family='Arial, sans-serif' font-size='14' fill='%23999'%3ENo Cover%3C/text%3E%3C/svg%3E" alt="No cover" class="book-cover">`
                }
            </div>
            <div class="book-info">
                ${genre ? `<div class="book-genre">${genre}</div>` : ''}
                <h3 class="book-title">${book.title}</h3>
                <p class="book-author">by ${book.author}</p>
                <div class="book-meta">
                    <span class="book-id">ID: ${bookId}</span>
                    <span class="book-copies ${isAvailable ? 'available' : 'unavailable'}">
                        ${isAvailable ? `${book.availableCopies || '1'} available` : 'Not available'}
                    </span>
                </div>
                ${book.downloadLink ? 
                    `<a href="${book.downloadLink}" target="_blank" class="btn-download">Download</a>` 
                    : ''}
                ${isAvailable ? 
                    `<button class="btn-borrow" onclick="borrowBook(${bookId})">Borrow Book</button>` : 
                    `<button class="btn-disabled" disabled>Not Available</button>`
                }
            </div>
        </div>
        `;
    }).join('');
}

function formatDate(dateString) {
    if (!dateString) return 'N/A';
    const date = new Date(dateString);
    return date.toLocaleDateString('en-US', { year: 'numeric', month: 'long', day: 'numeric' });
}

document.addEventListener('DOMContentLoaded', () => {
    const searchInput = document.getElementById('searchInput');
    if (searchInput) {
        searchInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                searchBooks();
            }
        });
    }
});

async function borrowBook(bookID) {
    if (!currentUserID) {
        alert('User ID not found. Please log in again.');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/borrow`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                userID: currentUserID,
                bookID: bookID
            })
        });
        
        const data = await response.json();
        
        if (response.ok && data.status === 'success') {
            const currentTotal = parseInt(localStorage.getItem(`user_${currentUserID}_totalBorrowed`) || '0');
            localStorage.setItem(`user_${currentUserID}_totalBorrowed`, (currentTotal + 1).toString());
            
            await loadAllBooks();
            await loadMyBorrowedBooks();
            await loadUserStatistics();
            alert('Book borrowed successfully!');
        } else {
            alert(data.message || 'Failed to borrow book');
        }
    } catch (error) {
        console.error('Error borrowing book:', error);
        alert('Error borrowing book: ' + error.message);
    }
}

async function borrowBookByID() {
    const bookIDInput = document.getElementById('borrowBookID');
    const bookID = parseInt(bookIDInput.value);
    
    if (!bookID || bookID <= 0) {
        alert('Please enter a valid Book ID');
        return;
    }
    
    await borrowBook(bookID);
    
    bookIDInput.value = '';
    loadMyBorrowedBooks();
}

async function returnBook(bookID) {
    if (!currentUserID) {
        alert('User ID not found. Please log in again.');
        return;
    }
    
    try {
        const response = await fetch(`${API_URL}/return`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                userID: currentUserID,
                bookID: bookID
            })
        });
        
        const data = await response.json();
        
        if (response.ok && data.status === 'success') {
            await loadMyBorrowedBooks();
            await loadAllBooks();
            await loadUserStatistics();
            alert('Book returned successfully!');
        } else {
            alert(data.message || 'Failed to return book');
        }
    } catch (error) {
        console.error('Error returning book:', error);
        alert('Error returning book: ' + error.message);
    }
}

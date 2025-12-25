// Global state
let allBooks = [];
let currentType = 'Novel';
let currentCategory = 'all';
let searchTerm = '';

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    setupEventListeners();
    loadBooks();
});

function setupEventListeners() {
    // Main type tabs
    document.querySelectorAll('.main-tab').forEach(tab => {
        tab.addEventListener('click', () => {
            document.querySelectorAll('.main-tab').forEach(t => t.classList.remove('active'));
            tab.classList.add('active');
            currentType = tab.dataset.type;
            filterAndDisplay();
        });
    });

    // Category buttons
    document.querySelectorAll('.category-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            document.querySelectorAll('.category-btn').forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
            currentCategory = btn.dataset.category;
            filterAndDisplay();
        });
    });

    // Search
    document.getElementById('searchInput').addEventListener('input', (e) => {
        searchTerm = e.target.value.toLowerCase();
        filterAndDisplay();
    });

    // Modal close
    document.querySelector('.close').addEventListener('click', () => {
        document.getElementById('bookModal').style.display = 'none';
    });

    window.addEventListener('click', (e) => {
        const modal = document.getElementById('bookModal');
        if (e.target === modal) {
            modal.style.display = 'none';
        }
    });
}

async function loadBooks() {
    try {
        const response = await fetch(`${API_BASE}/books`);
        const data = await response.json();
        
        if (data.status === 'success') {
            allBooks = data.data;
            document.getElementById('totalCount').textContent = `${allBooks.length} items`;
            filterAndDisplay();
        } else {
            showError('Failed to load books');
        }
    } catch (error) {
        console.error('Error:', error);
        showError('Error connecting to server');
    }
}

function filterAndDisplay() {
    let filtered = allBooks;

    // Filter by type (Novel, Comic, Manga)
    filtered = filtered.filter(book => {
        // Check if book has type field OR check category field
        if (book.type) {
            return book.type === currentType;
        } else if (book.category) {
            return book.category.startsWith(currentType);
        }
        return false;
    });

    // Filter by category
    if (currentCategory !== 'all') {
        filtered = filtered.filter(book => {
            return book.category && book.category.includes(currentCategory);
        });
    }

    // Filter by search term
    if (searchTerm) {
        filtered = filtered.filter(book => {
            return book.title.toLowerCase().includes(searchTerm) ||
                   book.author.toLowerCase().includes(searchTerm);
        });
    }

    displayBooks(filtered);
}

function displayBooks(books) {
    const grid = document.getElementById('booksGrid');
    
    if (books.length === 0) {
        grid.innerHTML = '<div class="no-results">No items found</div>';
        return;
    }

    grid.innerHTML = books.map(book => `
        <div class="book-card" onclick="showBookDetails(${book.id})">
            <div class="book-cover">
                ${book.coverImage ? `<img src="${book.coverImage}" alt="${book.title}" style="width:100%;height:100%;object-fit:cover;">` : book.title}
            </div>
            <div class="book-info">
                <div class="book-title">${book.title}</div>
                <div class="book-author">by ${book.author}</div>
                <span class="book-category">${book.category.split(' - ')[1] || book.category}</span>
                <div class="book-copies">📚 ${book.availableCopies}/${book.copies} available</div>
            </div>
        </div>
    `).join('');
}

function showBookDetails(bookId) {
    const book = allBooks.find(b => b.id === bookId);
    if (!book) return;

    const downloadLinks = book.downloadLinks || {};
    let downloadHTML = '';

    if (downloadLinks.epub) {
        downloadHTML += `<a href="${downloadLinks.epub}" class="download-btn" target="_blank">📖 Read EPUB</a>`;
    }
    if (downloadLinks.pdf) {
        downloadHTML += `<a href="${downloadLinks.pdf}" class="download-btn" target="_blank">📄 Read PDF</a>`;
    }
    if (downloadLinks.text) {
        downloadHTML += `<a href="${downloadLinks.text}" class="download-btn" target="_blank">📝 Read Text</a>`;
    }
    if (downloadLinks.web) {
        downloadHTML += `<a href="${downloadLinks.web}" class="download-btn" target="_blank">🌐 Read Online</a>`;
    }

    const modalBody = document.getElementById('modalBody');
    modalBody.innerHTML = `
        <div class="modal-header">
            <h2 class="modal-title">${book.title}</h2>
            <p class="modal-author">by ${book.author}</p>
        </div>
        <div class="modal-details">
            <div class="detail-row">
                <span class="detail-label">Category:</span> ${book.category}
            </div>
            <div class="detail-row">
                <span class="detail-label">ISBN:</span> ${book.isbn}
            </div>
            <div class="detail-row">
                <span class="detail-label">Copies Available:</span> ${book.availableCopies} of ${book.copies}
            </div>
            ${book.gutenbergId ? `<div class="detail-row">
                <span class="detail-label">Source:</span> Project Gutenberg #${book.gutenbergId}
            </div>` : ''}
        </div>
        ${downloadHTML ? `
            <div class="download-buttons">
                ${downloadHTML}
            </div>
        ` : '<p style="color: #888;">No download links available for this item.</p>'}
    `;

    document.getElementById('bookModal').style.display = 'block';
}

function showError(message) {
    document.getElementById('booksGrid').innerHTML = `
        <div class="no-results">${message}</div>
    `;
}

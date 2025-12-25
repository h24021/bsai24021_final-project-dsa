// Category Loader for Library Management System
// Loads books by category (Novel, Comic, Manga)

async function loadCategory(categoryType) {
    const container = document.getElementById('bookContainer');
    const loadingState = document.getElementById('loadingState');
    const errorState = document.getElementById('errorState');
    const emptyState = document.getElementById('emptyState');

    // Show loading state
    loadingState.style.display = 'block';
    container.style.display = 'none';
    errorState.style.display = 'none';
    emptyState.style.display = 'none';

    try {
        const response = await fetch(`${window.ENV.API_BASE}/books`);
        const data = await response.json();

        if (data.status === 'success' && data.data) {
            // Filter books by type or category
            const filteredBooks = data.data.filter(book => {
                // Check if book has type field
                if (book.type) {
                    return book.type === categoryType;
                }
                // Fallback: check if category starts with the type
                if (book.category) {
                    return book.category.startsWith(categoryType);
                }
                return false;
            });

            loadingState.style.display = 'none';

            if (filteredBooks.length === 0) {
                emptyState.style.display = 'block';
            } else {
                container.style.display = 'grid';
                displayBooks(filteredBooks);
            }
        } else {
            throw new Error('Invalid response format');
        }
    } catch (error) {
        console.error('Error loading books:', error);
        loadingState.style.display = 'none';
        errorState.style.display = 'block';
    }
}

function displayBooks(books) {
    const container = document.getElementById('bookContainer');
    container.innerHTML = '';

    books.forEach(book => {
        const bookItem = createBookElement(book);
        container.appendChild(bookItem);
    });
}

function createBookElement(book) {
    const item = document.createElement('div');
    item.className = 'book-item';

    const cover = document.createElement('div');
    cover.className = 'book-cover';

    if (book.coverImage && book.coverImage.trim() !== '') {
        const img = document.createElement('img');
        img.src = book.coverImage;
        img.alt = book.title || 'Book cover';
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
    title.textContent = book.title && book.title.trim() !== '' ? book.title : 'Untitled';

    const author = document.createElement('div');
    author.className = 'book-author';
    author.textContent = book.author && book.author.trim() !== '' ? book.author : 'Unknown Author';

    item.appendChild(cover);
    item.appendChild(title);
    item.appendChild(author);

    // Add click handler for download/read (if available)
    if (book.downloadLinks && book.downloadLinks.length > 0) {
        item.style.cursor = 'pointer';
        item.addEventListener('click', () => {
            // Open first available download link
            window.open(book.downloadLinks[0], '_blank');
        });
    }

    return item;
}

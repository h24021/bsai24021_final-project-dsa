#!/usr/bin/env python3
"""
Script to fetch real cover images for all books using multiple APIs:
1. Google Books API (primary source)
2. Open Library API (fallback)
3. Comic-specific sources for comics/manga
"""

import json
import requests
import time
from urllib.parse import quote

# API endpoints
GOOGLE_BOOKS_API = "https://www.googleapis.com/books/v1/volumes"
OPEN_LIBRARY_SEARCH = "https://openlibrary.org/search.json"
OPEN_LIBRARY_COVERS = "https://covers.openlibrary.org/b"

def fetch_google_books_cover(title, author):
    """Fetch cover from Google Books API"""
    try:
        query = f"{title} {author}".strip()
        params = {
            'q': query,
            'maxResults': 1,
            'printType': 'books'
        }
        
        response = requests.get(GOOGLE_BOOKS_API, params=params, timeout=5)
        if response.status_code == 200:
            data = response.json()
            if 'items' in data and len(data['items']) > 0:
                volume_info = data['items'][0].get('volumeInfo', {})
                image_links = volume_info.get('imageLinks', {})
                
                # Try different image sizes
                cover_url = (
                    image_links.get('extraLarge') or
                    image_links.get('large') or
                    image_links.get('medium') or
                    image_links.get('small') or
                    image_links.get('thumbnail')
                )
                
                if cover_url:
                    # Upgrade to higher quality if available
                    cover_url = cover_url.replace('zoom=1', 'zoom=2')
                    cover_url = cover_url.replace('&edge=curl', '')
                    return cover_url
    except Exception as e:
        print(f"  Google Books error: {e}")
    
    return None

def fetch_open_library_cover(title, author):
    """Fetch cover from Open Library API"""
    try:
        query = f"{title} {author}".strip()
        params = {
            'title': title,
            'author': author,
            'limit': 1
        }
        
        response = requests.get(OPEN_LIBRARY_SEARCH, params=params, timeout=5)
        if response.status_code == 200:
            data = response.json()
            if 'docs' in data and len(data['docs']) > 0:
                doc = data['docs'][0]
                
                # Try to get cover ID
                cover_i = doc.get('cover_i')
                if cover_i:
                    return f"https://covers.openlibrary.org/b/id/{cover_i}-L.jpg"
                
                # Try ISBN
                isbn = doc.get('isbn', [None])[0] if 'isbn' in doc else None
                if isbn:
                    return f"https://covers.openlibrary.org/b/isbn/{isbn}-L.jpg"
    except Exception as e:
        print(f"  Open Library error: {e}")
    
    return None

def fetch_comic_cover(title):
    """Fetch cover for comics/manga from Comic Vine or other sources"""
    # For now, try Google Books with "comic" or "manga" keyword
    try:
        query = f"{title} comic graphic novel"
        params = {
            'q': query,
            'maxResults': 1
        }
        
        response = requests.get(GOOGLE_BOOKS_API, params=params, timeout=5)
        if response.status_code == 200:
            data = response.json()
            if 'items' in data and len(data['items']) > 0:
                volume_info = data['items'][0].get('volumeInfo', {})
                image_links = volume_info.get('imageLinks', {})
                
                cover_url = (
                    image_links.get('large') or
                    image_links.get('medium') or
                    image_links.get('thumbnail')
                )
                
                if cover_url:
                    return cover_url.replace('zoom=1', 'zoom=2')
    except Exception as e:
        print(f"  Comic cover error: {e}")
    
    return None

def is_placeholder_cover(cover_url):
    """Check if cover URL is a placeholder"""
    if not cover_url:
        return True
    
    placeholder_patterns = [
        'default.jpg',
        'default-M.jpg',
        'default-L.jpg',
        '/files/default/',
        'placeholder'
    ]
    
    return any(pattern in cover_url for pattern in placeholder_patterns)

def update_book_covers(input_file='library_data.json', output_file='library_data_with_covers.json'):
    """Update all book covers with real images"""
    
    print("Loading library data...")
    with open(input_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    books = data.get('books', [])
    total_books = len(books)
    updated_count = 0
    failed_count = 0
    
    print(f"Found {total_books} books")
    print("Starting cover image updates...\n")
    
    for i, book in enumerate(books, 1):
        book_id = book.get('bookID')
        title = book.get('title', '')
        author = book.get('author', '')
        book_type = book.get('type', 'Novel')
        current_cover = book.get('cover', '')
        
        print(f"[{i}/{total_books}] {title}")
        
        # Skip if already has a real cover
        if not is_placeholder_cover(current_cover):
            print(f"  ✓ Already has cover: {current_cover[:60]}...")
            continue
        
        print(f"  Updating placeholder cover...")
        
        # Try different sources based on book type
        new_cover = None
        
        if book_type in ['Comic', 'Manga']:
            # Try comic-specific search first
            new_cover = fetch_comic_cover(title)
            if not new_cover:
                new_cover = fetch_google_books_cover(title, author)
            if not new_cover:
                new_cover = fetch_open_library_cover(title, author)
        else:
            # Try Google Books first for novels
            new_cover = fetch_google_books_cover(title, author)
            if not new_cover:
                new_cover = fetch_open_library_cover(title, author)
        
        if new_cover:
            book['cover'] = new_cover
            updated_count += 1
            print(f"  ✓ Updated: {new_cover[:60]}...")
        else:
            failed_count += 1
            print(f"  ✗ Failed to find cover")
        
        # Rate limiting
        if i % 10 == 0:
            print(f"\nProgress: {i}/{total_books} processed, {updated_count} updated, {failed_count} failed\n")
            time.sleep(1)  # Pause every 10 books
        else:
            time.sleep(0.2)  # Small delay between requests
    
    # Save updated data
    print(f"\nSaving updated data to {output_file}...")
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    
    print(f"\n{'='*60}")
    print(f"SUMMARY:")
    print(f"{'='*60}")
    print(f"Total books: {total_books}")
    print(f"Successfully updated: {updated_count}")
    print(f"Failed to find covers: {failed_count}")
    print(f"Already had covers: {total_books - updated_count - failed_count}")
    print(f"{'='*60}")
    print(f"\nUpdated data saved to: {output_file}")
    print(f"Review the file, then rename it to 'library_data.json' to use it.")

if __name__ == '__main__':
    update_book_covers()

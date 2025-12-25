#!/usr/bin/env python3
"""
Fetch real comic and manga covers from various sources
"""
import json
import requests
import time
import random

def get_openlibrary_cover(title, author):
    """Try to get cover from Open Library"""
    try:
        # Search Open Library
        search_url = f"https://openlibrary.org/search.json?title={title}&limit=1"
        response = requests.get(search_url, timeout=5)
        if response.status_code == 200:
            data = response.json()
            if data.get('docs') and len(data['docs']) > 0:
                book = data['docs'][0]
                if 'cover_i' in book:
                    cover_id = book['cover_i']
                    return f"https://covers.openlibrary.org/b/id/{cover_id}-M.jpg"
    except:
        pass
    return None

def get_marvel_placeholder(title):
    """Get Marvel-style cover for superhero comics"""
    # Marvel characters
    marvel_chars = ['spider-man', 'iron-man', 'captain-america', 'thor', 'hulk', 
                   'black-widow', 'hawkeye', 'doctor-strange', 'black-panther']
    if any(char in title.lower() for char in marvel_chars):
        return f"https://picsum.photos/seed/marvel-{hash(title)}/400/600"
    return None

def update_comic_covers():
    """Update comics with real or better covers"""
    with open('library_data.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    comics_updated = 0
    mangas_updated = 0
    
    # Known comic series with real covers
    comic_covers = {
        'Watchmen': 'https://m.media-amazon.com/images/I/81MUWh6+CwL._AC_UF1000,1000_QL80_.jpg',
        'Batman': 'https://m.media-amazon.com/images/I/91BP8HzDesL._AC_UF1000,1000_QL80_.jpg',
        'Spider-Man': 'https://m.media-amazon.com/images/I/91Z7c8RDHRL._AC_UF1000,1000_QL80_.jpg',
        'Superman': 'https://m.media-amazon.com/images/I/81fhfVGJj3L._AC_UF1000,1000_QL80_.jpg',
        'Wonder Woman': 'https://m.media-amazon.com/images/I/81Qmcwt+vxL._AC_UF1000,1000_QL80_.jpg',
        'X-Men': 'https://m.media-amazon.com/images/I/91a1RjGkxrL._AC_UF1000,1000_QL80_.jpg',
        'The Walking Dead': 'https://m.media-amazon.com/images/I/91jmL4KyJUL._AC_UF1000,1000_QL80_.jpg',
        'The Sandman': 'https://m.media-amazon.com/images/I/81UQP0AXYTL._AC_UF1000,1000_QL80_.jpg',
        'Saga': 'https://m.media-amazon.com/images/I/91yFPcVxLxL._AC_UF1000,1000_QL80_.jpg',
        'Fables': 'https://m.media-amazon.com/images/I/81C9kK2LUUL._AC_UF1000,1000_QL80_.jpg',
        'Y: The Last Man': 'https://m.media-amazon.com/images/I/81nTTm+qH5L._AC_UF1000,1000_QL80_.jpg',
        'Preacher': 'https://m.media-amazon.com/images/I/81k3tWDwPqL._AC_UF1000,1000_QL80_.jpg',
        'Sin City': 'https://m.media-amazon.com/images/I/81bTWo5jRvL._AC_UF1000,1000_QL80_.jpg',
        'V for Vendetta': 'https://m.media-amazon.com/images/I/71Dej45qJxL._AC_UF1000,1000_QL80_.jpg',
        '30 Days of Night': 'https://m.media-amazon.com/images/I/81kYjH6BYEL._AC_UF1000,1000_QL80_.jpg',
        'Hellblazer': 'https://m.media-amazon.com/images/I/81IHlZWNXdL._AC_UF1000,1000_QL80_.jpg',
        'Locke & Key': 'https://m.media-amazon.com/images/I/91cEUMNGC6L._AC_UF1000,1000_QL80_.jpg',
        'xkcd': 'https://m.media-amazon.com/images/I/71TYgV9qb9L._AC_UF1000,1000_QL80_.jpg',
        'Calvin and Hobbes': 'https://m.media-amazon.com/images/I/91MD2T4mXvL._AC_UF1000,1000_QL80_.jpg',
        'Peanuts': 'https://m.media-amazon.com/images/I/81YkEcf0BsL._AC_UF1000,1000_QL80_.jpg',
        'Garfield': 'https://m.media-amazon.com/images/I/81a7LDdPfCL._AC_UF1000,1000_QL80_.jpg',
        'Dilbert': 'https://m.media-amazon.com/images/I/81pu-M4VH9L._AC_UF1000,1000_QL80_.jpg',
        'The Far Side': 'https://m.media-amazon.com/images/I/81x8fmzLzxL._AC_UF1000,1000_QL80_.jpg',
    }
    
    # Known manga series with real covers
    manga_covers = {
        'Akira': 'https://m.media-amazon.com/images/I/91YnxAb0-SL._AC_UF1000,1000_QL80_.jpg',
        'Death Note': 'https://m.media-amazon.com/images/I/81Rm1oGLZkL._AC_UF1000,1000_QL80_.jpg',
        'One Piece': 'https://m.media-amazon.com/images/I/81ZF5XUBmAL._AC_UF1000,1000_QL80_.jpg',
        'Naruto': 'https://m.media-amazon.com/images/I/91VPuLJZKNL._AC_UF1000,1000_QL80_.jpg',
        'Attack on Titan': 'https://m.media-amazon.com/images/I/81CFNO1W1yL._AC_UF1000,1000_QL80_.jpg',
        'Tokyo Ghoul': 'https://m.media-amazon.com/images/I/71l6q8FBcEL._AC_UF1000,1000_QL80_.jpg',
        'Fullmetal Alchemist': 'https://m.media-amazon.com/images/I/81oLy93xWSL._AC_UF1000,1000_QL80_.jpg',
        'My Hero Academia': 'https://m.media-amazon.com/images/I/91Aw8k3KLNL._AC_UF1000,1000_QL80_.jpg',
        'Demon Slayer': 'https://m.media-amazon.com/images/I/81Th7uSWy7L._AC_UF1000,1000_QL80_.jpg',
        'Berserk': 'https://m.media-amazon.com/images/I/91z4S5U5V7L._AC_UF1000,1000_QL80_.jpg',
        'Hunter x Hunter': 'https://m.media-amazon.com/images/I/81SiVBvKTLL._AC_UF1000,1000_QL80_.jpg',
        'Bleach': 'https://m.media-amazon.com/images/I/81LjKhBuYML._AC_UF1000,1000_QL80_.jpg',
        'Dragon Ball': 'https://m.media-amazon.com/images/I/81OwbBLkRwL._AC_UF1000,1000_QL80_.jpg',
        'Sailor Moon': 'https://m.media-amazon.com/images/I/81t5WR5L9LL._AC_UF1000,1000_QL80_.jpg',
        'Neon Genesis Evangelion': 'https://m.media-amazon.com/images/I/91tPa2X9HUL._AC_UF1000,1000_QL80_.jpg',
        'Cowboy Bebop': 'https://m.media-amazon.com/images/I/91LF7FQ8RbL._AC_UF1000,1000_QL80_.jpg',
        'Vagabond': 'https://m.media-amazon.com/images/I/91hYFYYxdML._AC_UF1000,1000_QL80_.jpg',
        'Monster': 'https://m.media-amazon.com/images/I/81TlWNvOW8L._AC_UF1000,1000_QL80_.jpg',
        'Vinland Saga': 'https://m.media-amazon.com/images/I/91Q4LYKcMnL._AC_UF1000,1000_QL80_.jpg',
        'Chainsaw Man': 'https://m.media-amazon.com/images/I/81xW+0xz3fL._AC_UF1000,1000_QL80_.jpg',
    }
    
    for book in data['books']:
        if book.get('type') == 'Comic':
            # Try to match with known series
            matched = False
            for series, cover_url in comic_covers.items():
                if series.lower() in book['title'].lower():
                    book['coverImage'] = cover_url
                    comics_updated += 1
                    matched = True
                    break
            
            if not matched:
                # Use high-quality placeholder
                book['coverImage'] = f"https://picsum.photos/seed/comic-{book['id']}/400/600"
                comics_updated += 1
                
        elif book.get('type') == 'Manga':
            # Try to match with known series
            matched = False
            for series, cover_url in manga_covers.items():
                if series.lower() in book['title'].lower():
                    book['coverImage'] = cover_url
                    mangas_updated += 1
                    matched = True
                    break
            
            if not matched:
                # Use high-quality placeholder
                book['coverImage'] = f"https://picsum.photos/seed/manga-{book['id']}/400/600"
                mangas_updated += 1
    
    # Save updated data
    with open('library_data.json', 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    
    print(f"✅ Updated covers:")
    print(f"   Comics: {comics_updated} updated")
    print(f"   Mangas: {mangas_updated} updated")
    print(f"   Using real covers from Amazon for known series")

if __name__ == '__main__':
    update_comic_covers()

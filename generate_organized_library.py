#!/usr/bin/env python3
"""
Generate properly categorized library with Novels, Comics, and Manga
Categories: Fiction, Horror, Thriller, Romance, Psychological, Comedy, Classic
"""
import json
import requests
import time
import random

# Proper categories
CATEGORIES = ["Fiction", "Horror", "Thriller", "Romance", "Psychological", "Comedy", "Classic"]

def fetch_gutenberg_novels_by_category(total=500):
    """Fetch real novels from Project Gutenberg organized by category"""
    novels = []
    book_id = 1001
    
    # Category mapping for Project Gutenberg subjects
    category_keywords = {
        "Horror": ["horror", "gothic", "ghost", "supernatural", "terror"],
        "Romance": ["romance", "love stories", "romantic"],
        "Thriller": ["mystery", "detective", "crime", "suspense", "thriller"],
        "Fiction": ["fiction", "novel", "literature"],
        "Psychological": ["psychological", "philosophy", "mental"],
        "Comedy": ["humor", "humorous", "comedy", "satire"],
        "Classic": ["classic", "literature"]
    }
    
    print(f"Fetching {total} novels from Project Gutenberg...")
    
    # Fetch novels and categorize them
    page = 1
    attempts = 0
    max_attempts = 50
    
    while len(novels) < total and attempts < max_attempts:
        try:
            url = f"https://gutendex.com/books?page={page}"
            response = requests.get(url, timeout=10)
            response.raise_for_status()
            data = response.json()
            
            for item in data.get('results', []):
                if len(novels) >= total:
                    break
                
                title = item.get('title', 'Unknown Title')
                authors = item.get('authors', [])
                author = authors[0].get('name', 'Unknown Author') if authors else 'Unknown Author'
                
                # Categorize based on subjects
                subjects = ' '.join(item.get('subjects', [])).lower()
                assigned_category = "Fiction"  # default
                
                for category, keywords in category_keywords.items():
                    if any(keyword in subjects for keyword in keywords):
                        assigned_category = category
                        break
                
                # Get formats
                formats = item.get('formats', {})
                epub_url = formats.get('application/epub+zip', '')
                pdf_url = formats.get('application/pdf', '')
                text_url = formats.get('text/plain; charset=utf-8', '')
                cover_url = formats.get('image/jpeg', '')
                
                gutenberg_id = item.get('id', book_id)
                isbn = f"PG-{gutenberg_id:06d}"
                copies = random.randint(2, 8)
                
                novels.append({
                    "id": book_id,
                    "title": title,
                    "author": author,
                    "isbn": isbn,
                    "category": f"Novel - {assigned_category}",
                    "copies": copies,
                    "availableCopies": copies,
                    "coverImage": cover_url,
                    "downloadLinks": {
                        "epub": epub_url,
                        "pdf": pdf_url,
                        "text": text_url
                    },
                    "gutenbergId": gutenberg_id,
                    "type": "Novel"
                })
                
                book_id += 1
                
                if len(novels) % 50 == 0:
                    print(f"  Fetched {len(novels)} novels...")
            
            page += 1
            attempts += 1
            time.sleep(0.5)
            
        except Exception as e:
            print(f"  Error fetching page {page}: {e}")
            attempts += 1
            time.sleep(1)
    
    print(f"✓ Fetched {len(novels)} novels")
    return novels, book_id

def generate_manga_data(start_id, count=200):
    """Generate manga with proper categories"""
    manga = []
    
    print(f"Generating {count} manga...")
    
    # Popular manga titles by category
    manga_titles = {
        "Horror": ["Tokyo Ghoul", "Another", "Parasyte", "Deadman Wonderland", "Higurashi"],
        "Thriller": ["Death Note", "Monster", "Psycho-Pass", "Erased", "Promised Neverland"],
        "Romance": ["Your Lie in April", "Kimi ni Todoke", "Horimiya", "Fruits Basket", "Lovely Complex"],
        "Comedy": ["One Punch Man", "Gintama", "Nichijou", "Daily Lives of High School Boys", "Kaguya-sama"],
        "Fiction": ["Fullmetal Alchemist", "Attack on Titan", "My Hero Academia", "Demon Slayer", "Naruto"],
        "Psychological": ["Monster", "Oyasumi Punpun", "Homunculus", "Liar Game", "Kaiji"],
        "Classic": ["Akira", "Dragon Ball", "Sailor Moon", "Astro Boy", "Ghost in the Shell"]
    }
    
    for category in CATEGORIES:
        titles = manga_titles.get(category, ["Manga Series"])
        per_category = count // len(CATEGORIES)
        
        for i in range(per_category):
            manga_id = start_id + len(manga)
            base_title = random.choice(titles)
            volume = random.randint(1, 50)
            title = f"{base_title} Vol. {volume}"
            
            manga.append({
                "id": manga_id,
                "title": title,
                "author": "Various Manga Artists",
                "isbn": f"MANGA-{manga_id:06d}",
                "category": f"Manga - {category}",
                "copies": random.randint(5, 20),
                "availableCopies": random.randint(3, 20),
                "coverImage": f"https://via.placeholder.com/400x600?text={title.replace(' ', '+')}",
                "downloadLinks": {
                    "info": "Manga reading available online"
                },
                "type": "Manga"
            })
    
    print(f"✓ Generated {len(manga)} manga across {len(CATEGORIES)} categories")
    return manga, start_id + len(manga)

def generate_comics_data(start_id, count=300):
    """Generate comics with proper categories"""
    comics = []
    
    print(f"Generating {count} comics...")
    
    # Real xkcd comics (100)
    xkcd_count = 100
    for i in range(1, xkcd_count + 1):
        comics.append({
            "id": start_id + i - 1,
            "title": f"xkcd #{i}",
            "author": "Randall Munroe",
            "isbn": f"XKCD-{i:06d}",
            "category": "Comic - Comedy",
            "copies": 999,
            "availableCopies": 999,
            "coverImage": f"https://imgs.xkcd.com/{i}/",
            "downloadLinks": {
                "web": f"https://xkcd.com/{i}/"
            },
            "type": "Comic"
        })
    
    # Popular comic series by category
    comic_series = {
        "Horror": ["The Walking Dead", "30 Days of Night", "Locke & Key", "Sandman", "Hellblazer"],
        "Thriller": ["Watchmen", "V for Vendetta", "Sin City", "The Killing Joke", "Batman"],
        "Romance": ["Archie", "Love and Rockets", "Saga", "Heartstopper", "Lumberjanes"],
        "Comedy": ["Garfield", "Calvin and Hobbes", "Peanuts", "The Far Side", "Dilbert"],
        "Fiction": ["The Sandman", "Fables", "Y: The Last Man", "Preacher", "Saga"],
        "Psychological": ["Black Mirror", "Sandman", "Swamp Thing", "The Invisibles", "The Maxx"],
        "Classic": ["Superman", "Batman", "Spider-Man", "Wonder Woman", "X-Men"]
    }
    
    remaining = count - xkcd_count
    start_pos = start_id + xkcd_count
    
    for category in CATEGORIES:
        series_list = comic_series.get(category, ["Comic Series"])
        per_category = remaining // len(CATEGORIES)
        
        for i in range(per_category):
            comic_id = start_pos + len(comics) - xkcd_count
            series = random.choice(series_list)
            issue = random.randint(1, 200)
            title = f"{series} #{issue}"
            
            comics.append({
                "id": comic_id,
                "title": title,
                "author": "Various Comic Artists",
                "isbn": f"COMIC-{comic_id:06d}",
                "category": f"Comic - {category}",
                "copies": random.randint(10, 30),
                "availableCopies": random.randint(5, 30),
                "coverImage": f"https://via.placeholder.com/400x600?text={title.replace(' ', '+').replace('#', '%23')}",
                "downloadLinks": {
                    "info": "Comic reading available"
                },
                "type": "Comic"
            })
    
    print(f"✓ Generated {len(comics)} comics ({xkcd_count} real xkcd + {remaining} others)")
    return comics

def generate_users(num_users=50):
    """Generate sample users"""
    users = []
    first_names = ["John", "Jane", "Mike", "Sarah", "Tom", "Lisa", "David", "Emily", "Chris", "Anna"]
    last_names = ["Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez"]
    
    for i in range(num_users):
        user_id = 2001 + i
        first = random.choice(first_names)
        last = random.choice(last_names)
        name = f"{first} {last}"
        email = f"{first.lower()}.{last.lower()}{i}@library.com"
        role = "member" if i > 2 else "librarian"
        
        users.append({
            "id": user_id,
            "name": name,
            "email": email,
            "role": role
        })
    
    return users

if __name__ == "__main__":
    print("=" * 70)
    print("Generating Organized Library: Novels, Comics, Manga")
    print("Categories: Fiction, Horror, Thriller, Romance, Psychological, Comedy, Classic")
    print("=" * 70)
    
    # Generate data
    novels, next_id = fetch_gutenberg_novels_by_category(500)
    manga, next_id = generate_manga_data(next_id, 200)
    comics = generate_comics_data(next_id, 300)
    users = generate_users(50)
    
    all_books = novels + manga + comics
    
    data = {
        "books": all_books,
        "users": users
    }
    
    # Save to JSON
    with open("library_data.json", "w") as f:
        json.dump(data, f, indent=2)
    
    # Print summary
    print("\n" + "=" * 70)
    print(f"✓ Total items: {len(all_books)}")
    print(f"\nNovels: {len(novels)}")
    for cat in CATEGORIES:
        count = sum(1 for b in novels if f"Novel - {cat}" in b['category'])
        print(f"  - {cat}: {count}")
    
    print(f"\nManga: {len(manga)}")
    for cat in CATEGORIES:
        count = sum(1 for b in manga if f"Manga - {cat}" in b['category'])
        print(f"  - {cat}: {count}")
    
    print(f"\nComics: {len(comics)}")
    for cat in CATEGORIES:
        count = sum(1 for b in comics if f"Comic - {cat}" in b['category'])
        print(f"  - {cat}: {count}")
    
    print(f"\n✓ Users: {len(users)}")
    print(f"✓ Saved to library_data.json")
    print("=" * 70)

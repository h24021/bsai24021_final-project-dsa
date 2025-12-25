#!/usr/bin/env python3
"""
Generate a large dataset of books, magazines, novels, and comics
"""
import json
import random

# Categories and sample data
categories = {
    "Novel": ["Fiction", "Mystery", "Thriller", "Romance", "Historical Fiction", "Science Fiction", "Fantasy"],
    "Book": ["Non-Fiction", "Biography", "Self-Help", "Business", "Science", "History", "Philosophy"],
    "Magazine": ["Technology", "Fashion", "Sports", "News", "Entertainment", "Health", "Travel"],
    "Comic": ["Superhero", "Manga", "Graphic Novel", "Adventure", "Comedy", "Horror"]
}

# Sample titles, authors, and publishers
novel_titles = [
    "The Silent Echo", "Shadows of Tomorrow", "The Last Kingdom", "Whispers in the Dark",
    "Beyond the Horizon", "The Crimson Path", "Echoes of Eternity", "The Midnight Garden",
    "Flames of Destiny", "The Crystal Tower", "Winds of Change", "The Silver Sword",
    "Tales of the Forgotten", "The Azure Sky", "Secrets of the Past", "The Golden Crown"
]

book_titles = [
    "Mastering Success", "The Science of Everything", "History Unfolded", "Think Like a Genius",
    "The Art of Living", "Understanding the Universe", "Philosophy for Beginners", "The Business Mind",
    "Journey Through Time", "The Human Story", "Principles of Life", "The Modern World"
]

magazine_titles = [
    "Tech Today", "Fashion Forward", "Sports Illustrated", "Global News", "Entertainment Weekly",
    "Health & Wellness", "Travel & Leisure", "Digital World", "Style Magazine", "Fitness First"
]

comic_titles = [
    "Super Squad", "The Dark Avenger", "Cosmic Warriors", "Thunder Strike", "Shadow Ninja",
    "The Amazing Adventures", "Power Rangers", "Mystery Man", "The Incredible Journey", "Star Heroes"
]

authors = [
    "James Anderson", "Sarah Mitchell", "Michael Chen", "Emily Davis", "Robert Johnson",
    "Jennifer Lee", "David Wilson", "Laura Martinez", "Christopher Brown", "Amanda Taylor",
    "Daniel Garcia", "Jessica White", "Matthew Rodriguez", "Ashley Thomas", "Andrew Jackson",
    "Sophia Lopez", "Joshua Hernandez", "Olivia Gonzalez", "Ryan Moore", "Emma Martin"
]

publishers = [
    "Penguin Books", "HarperCollins", "Random House", "Simon & Schuster", "Macmillan",
    "Hachette", "Scholastic", "Oxford Press", "Cambridge University Press", "Vintage Books"
]

def generate_books(num_books=1000):
    """Generate a list of books with varied types"""
    books = []
    book_id = 1001
    
    # Calculate distribution
    novels = int(num_books * 0.4)  # 40% novels
    other_books = int(num_books * 0.3)  # 30% other books
    magazines = int(num_books * 0.2)  # 20% magazines
    comics = num_books - novels - other_books - magazines  # Rest are comics
    
    # Generate novels
    for i in range(novels):
        category = random.choice(categories["Novel"])
        title = f"{random.choice(novel_titles)} Vol. {random.randint(1, 10)}"
        author = random.choice(authors)
        isbn = f"ISBN-{book_id:06d}"
        copies = random.randint(1, 10)
        
        books.append({
            "id": book_id,
            "title": title,
            "author": author,
            "isbn": isbn,
            "category": f"Novel - {category}",
            "copies": copies,
            "availableCopies": copies
        })
        book_id += 1
    
    # Generate other books
    for i in range(other_books):
        category = random.choice(categories["Book"])
        title = f"{random.choice(book_titles)} Edition {random.randint(1, 5)}"
        author = random.choice(authors)
        isbn = f"ISBN-{book_id:06d}"
        copies = random.randint(1, 8)
        
        books.append({
            "id": book_id,
            "title": title,
            "author": author,
            "isbn": isbn,
            "category": f"Book - {category}",
            "copies": copies,
            "availableCopies": copies
        })
        book_id += 1
    
    # Generate magazines
    for i in range(magazines):
        category = random.choice(categories["Magazine"])
        title = f"{random.choice(magazine_titles)} - {random.choice(['Monthly', 'Weekly', 'Quarterly'])}"
        author = f"{random.choice(publishers)} Editorial Team"
        isbn = f"ISSN-{book_id:06d}"
        copies = random.randint(5, 20)  # Magazines have more copies
        
        books.append({
            "id": book_id,
            "title": title,
            "author": author,
            "isbn": isbn,
            "category": f"Magazine - {category}",
            "copies": copies,
            "availableCopies": copies
        })
        book_id += 1
    
    # Generate comics
    for i in range(comics):
        category = random.choice(categories["Comic"])
        title = f"{random.choice(comic_titles)} Issue #{random.randint(1, 100)}"
        author = random.choice(authors)
        isbn = f"COMIC-{book_id:06d}"
        copies = random.randint(2, 15)
        
        books.append({
            "id": book_id,
            "title": title,
            "author": author,
            "isbn": isbn,
            "category": f"Comic - {category}",
            "copies": copies,
            "availableCopies": copies
        })
        book_id += 1
    
    return books

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
    # Generate data
    print("Generating library data...")
    books = generate_books(1000)
    users = generate_users(50)
    
    data = {
        "books": books,
        "users": users
    }
    
    # Save to JSON file
    with open("library_data.json", "w") as f:
        json.dump(data, f, indent=2)
    
    print(f"✓ Generated {len(books)} books")
    print(f"  - Novels: {sum(1 for b in books if 'Novel' in b['category'])}")
    print(f"  - Books: {sum(1 for b in books if b['category'].startswith('Book'))}")
    print(f"  - Magazines: {sum(1 for b in books if 'Magazine' in b['category'])}")
    print(f"  - Comics: {sum(1 for b in books if 'Comic' in b['category'])}")
    print(f"✓ Generated {len(users)} users")
    print(f"✓ Saved to library_data.json")

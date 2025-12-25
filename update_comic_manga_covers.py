#!/usr/bin/env python3
"""
Update comics and manga with real cover images
"""
import json

# Read the current library data
with open('library_data.json', 'r', encoding='utf-8') as f:
    data = json.load(f)

# Update comics and manga with better placeholder images
# Using imgur placeholders that actually work
for book in data['books']:
    if book.get('type') == 'Comic':
        # Use a comic book style placeholder
        book['coverImage'] = f"https://picsum.photos/seed/{book['id']}/400/600"
    elif book.get('type') == 'Manga':
        # Use a manga style placeholder
        book['coverImage'] = f"https://picsum.photos/seed/manga{book['id']}/400/600"

# Save the updated data
with open('library_data.json', 'w', encoding='utf-8') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)

print("✅ Updated comic and manga covers with picsum.photos placeholders")
print(f"   Total books: {len(data['books'])}")
comics = len([b for b in data['books'] if b.get('type') == 'Comic'])
mangas = len([b for b in data['books'] if b.get('type') == 'Manga'])
print(f"   Comics: {comics}, Mangas: {mangas}")

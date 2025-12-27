#!/usr/bin/env python3
"""
Add more unique famous books to reach 1000 total
"""

import json

def add_more_famous_books():
    # Load existing data
    with open('library_data.json', 'r') as f:
        data = json.load(f)
    
    existing_books = data['books']
    max_id = max(book['bookID'] for book in existing_books)
    
    # Track existing titles to avoid duplicates
    existing_titles = {book['title'] for book in existing_books}
    
    new_books = []
    book_id = max_id + 1
    
    # More Classic Novels
    classics = [
        ("The Count of Monte Cristo", "Dumas, Alexandre", "1184", "Novel - Adventure"),
        ("The Three Musketeers", "Dumas, Alexandre", "1257", "Novel - Adventure"),
        ("Twenty Thousand Leagues Under the Sea", "Verne, Jules", "164", "Novel - Sci-Fi"),
        ("Around the World in Eighty Days", "Verne, Jules", "103", "Novel - Adventure"),
        ("Les Misérables", "Hugo, Victor", "135", "Novel - Classic"),
        ("The Hunchback of Notre Dame", "Hugo, Victor", "2610", "Novel - Classic"),
        ("Crime and Punishment", "Dostoevsky, Fyodor", "2554", "Novel - Classic"),
        ("The Brothers Karamazov", "Dostoevsky, Fyodor", "28054", "Novel - Classic"),
        ("Anna Karenina", "Tolstoy, Leo", "1399", "Novel - Classic"),
        ("War and Peace", "Tolstoy, Leo", "2600", "Novel - Historical"),
        ("The Idiot", "Dostoevsky, Fyodor", "2638", "Novel - Classic"),
        ("Dead Souls", "Gogol, Nikolai", "1081", "Novel - Classic"),
        ("Don Quixote", "Cervantes, Miguel de", "996", "Novel - Classic"),
        ("The Odyssey", "Homer", "1727", "Novel - Classic"),
        ("The Iliad", "Homer", "6130", "Novel - Classic"),
        ("The Divine Comedy", "Dante Alighieri", "8800", "Novel - Classic"),
        ("Paradise Lost", "Milton, John", "26", "Novel - Classic"),
        ("Candide", "Voltaire", "19942", "Novel - Classic"),
        ("Robinson Crusoe", "Defoe, Daniel", "521", "Novel - Adventure"),
        ("Gulliver's Travels", "Swift, Jonathan", "829", "Novel - Fantasy"),
    ]
    
    # American Literature
    american_lit = [
        ("The Scarlet Letter", "Hawthorne, Nathaniel", "25", "Novel - Classic"),
        ("Moby Dick", "Melville, Herman", "2701", "Novel - Adventure"),
        ("The Last of the Mohicans", "Cooper, James Fenimore", "27", "Novel - Adventure"),
        ("Uncle Tom's Cabin", "Stowe, Harriet Beecher", "203", "Novel - Historical"),
        ("The Adventures of Tom Sawyer", "Twain, Mark", "74", "Novel - Adventure"),
        ("Life on the Mississippi", "Twain, Mark", "245", "Novel - Classic"),
        ("The Red Badge of Courage", "Crane, Stephen", "73", "Novel - War"),
        ("Sister Carrie", "Dreiser, Theodore", "233", "Novel - Classic"),
        ("The Age of Innocence", "Wharton, Edith", "541", "Novel - Classic"),
        ("Ethan Frome", "Wharton, Edith", "4517", "Novel - Classic"),
        ("The House of Mirth", "Wharton, Edith", "284", "Novel - Classic"),
        ("Main Street", "Lewis, Sinclair", "543", "Novel - Classic"),
        ("Babbitt", "Lewis, Sinclair", "1156", "Novel - Classic"),
    ]
    
    # British Novels
    british = [
        ("Treasure Island", "Stevenson, Robert Louis", "120", "Novel - Adventure"),
        ("Kidnapped", "Stevenson, Robert Louis", "421", "Novel - Adventure"),
        ("The Strange Case of Dr. Jekyll and Mr. Hyde", "Stevenson, Robert Louis", "43", "Novel - Horror"),
        ("Vanity Fair", "Thackeray, William Makepeace", "599", "Novel - Classic"),
        ("Tess of the d'Urbervilles", "Hardy, Thomas", "110", "Novel - Classic"),
        ("The Mayor of Casterbridge", "Hardy, Thomas", "143", "Novel - Classic"),
        ("Far from the Madding Crowd", "Hardy, Thomas", "107", "Novel - Classic"),
        ("The Return of the Native", "Hardy, Thomas", "122", "Novel - Classic"),
        ("Tom Jones", "Fielding, Henry", "6593", "Novel - Classic"),
        ("Clarissa", "Richardson, Samuel", "9296", "Novel - Classic"),
        ("Ivanhoe", "Scott, Walter", "82", "Novel - Historical"),
        ("Rob Roy", "Scott, Walter", "7025", "Novel - Historical"),
        ("The Woman in White", "Collins, Wilkie", "583", "Novel - Mystery"),
        ("The Moonstone", "Collins, Wilkie", "155", "Novel - Mystery"),
    ]
    
    # Science Fiction & Fantasy
    scifi_fantasy = [
        ("The Time Machine", "Wells, H. G.", "35", "Novel - Sci-Fi"),
        ("The Invisible Man", "Wells, H. G.", "5230", "Novel - Sci-Fi"),
        ("The Island of Doctor Moreau", "Wells, H. G.", "159", "Novel - Sci-Fi"),
        ("A Princess of Mars", "Burroughs, Edgar Rice", "62", "Novel - Sci-Fi"),
        ("The Gods of Mars", "Burroughs, Edgar Rice", "64", "Novel - Sci-Fi"),
        ("The Warlord of Mars", "Burroughs, Edgar Rice", "68", "Novel - Sci-Fi"),
        ("At the Earth's Core", "Burroughs, Edgar Rice", "123", "Novel - Sci-Fi"),
        ("The Land That Time Forgot", "Burroughs, Edgar Rice", "551", "Novel - Sci-Fi"),
        ("The Lost World", "Doyle, Arthur Conan", "139", "Novel - Sci-Fi"),
        ("The Poison Belt", "Doyle, Arthur Conan", "126", "Novel - Sci-Fi"),
        ("The King in Yellow", "Chambers, Robert W.", "8492", "Novel - Horror"),
        ("The Night Land", "Hodgson, William Hope", "10662", "Novel - Horror"),
        ("The House on the Borderland", "Hodgson, William Hope", "10002", "Novel - Horror"),
    ]
    
    # Mystery & Detective
    mystery = [
        ("The Maltese Falcon", "Hammett, Dashiell", "gutenberg", "Novel - Mystery"),
        ("The Big Sleep", "Chandler, Raymond", "gutenberg", "Novel - Mystery"),
        ("The Thin Man", "Hammett, Dashiell", "gutenberg", "Novel - Mystery"),
        ("The 39 Steps", "Buchan, John", "558", "Novel - Mystery"),
        ("The Thirty-Nine Steps", "Buchan, John", "558", "Novel - Thriller"),
        ("The Riddle of the Sands", "Childers, Erskine", "1906", "Novel - Thriller"),
    ]
    
    # Horror & Gothic
    horror = [
        ("The Picture of Dorian Gray", "Wilde, Oscar", "174", "Novel - Horror"),
        ("The Phantom of the Opera", "Leroux, Gaston", "175", "Novel - Horror"),
        ("The Mysteries of Udolpho", "Radcliffe, Ann", "3268", "Novel - Gothic"),
        ("The Castle of Otranto", "Walpole, Horace", "696", "Novel - Gothic"),
        ("The Monk", "Lewis, Matthew Gregory", "601", "Novel - Gothic"),
        ("Carmilla", "Le Fanu, Sheridan", "10007", "Novel - Horror"),
        ("The Turn of the Screw", "James, Henry", "209", "Novel - Horror"),
    ]
    
    # More Comics & Graphic Novels
    comics = [
        ("V for Vendetta", "Moore, Alan", "olid", "Comic - Superhero"),
        ("Saga Vol. 1", "Vaughan, Brian K.", "olid", "Comic - Fantasy"),
        ("The Sandman Vol. 1", "Gaiman, Neil", "olid", "Comic - Fantasy"),
        ("Fables Vol. 1", "Willingham, Bill", "olid", "Comic - Fantasy"),
        ("Y: The Last Man Vol. 1", "Vaughan, Brian K.", "olid", "Comic - Sci-Fi"),
        ("Preacher Vol. 1", "Ennis, Garth", "olid", "Comic - Horror"),
        ("Transmetropolitan Vol. 1", "Ellis, Warren", "olid", "Comic - Sci-Fi"),
        ("100 Bullets Vol. 1", "Azzarello, Brian", "olid", "Comic - Crime"),
        ("Planetary Vol. 1", "Ellis, Warren", "olid", "Comic - Sci-Fi"),
        ("The Authority Vol. 1", "Ellis, Warren", "olid", "Comic - Superhero"),
        ("Hellblazer Vol. 1", "Delano, Jamie", "olid", "Comic - Horror"),
        ("Swamp Thing Vol. 1", "Moore, Alan", "olid", "Comic - Horror"),
        ("Maus", "Spiegelman, Art", "olid", "Comic - Historical"),
        ("Persepolis", "Satrapi, Marjane", "olid", "Comic - Biography"),
        ("Fun Home", "Bechdel, Alison", "olid", "Comic - Biography"),
    ]
    
    # More Manga
    manga = [
        ("Fullmetal Alchemist Vol. 1", "Arakawa, Hiromu", "olid", "Manga - Fantasy"),
        ("Death Note Vol. 1", "Ohba, Tsugumi", "olid", "Manga - Thriller"),
        ("Cowboy Bebop Vol. 1", "Yatate, Hajime", "olid", "Manga - Sci-Fi"),
        ("Trigun Vol. 1", "Nightow, Yasuhiro", "olid", "Manga - Sci-Fi"),
        ("Neon Genesis Evangelion Vol. 1", "Sadamoto, Yoshiyuki", "olid", "Manga - Sci-Fi"),
        ("Ghost in the Shell", "Shirow, Masamune", "olid", "Manga - Sci-Fi"),
        ("Hellsing Vol. 1", "Hirano, Kouta", "olid", "Manga - Horror"),
        ("Vagabond Vol. 1", "Inoue, Takehiko", "olid", "Manga - Historical"),
        ("Vinland Saga Vol. 1", "Yukimura, Makoto", "olid", "Manga - Historical"),
        ("Monster Vol. 1", "Urasawa, Naoki", "olid", "Manga - Thriller"),
        ("20th Century Boys Vol. 1", "Urasawa, Naoki", "olid", "Manga - Mystery"),
        ("Pluto Vol. 1", "Urasawa, Naoki", "olid", "Manga - Sci-Fi"),
        ("Slam Dunk Vol. 1", "Inoue, Takehiko", "olid", "Manga - Sports"),
        ("Hunter x Hunter Vol. 1", "Togashi, Yoshihiro", "olid", "Manga - Adventure"),
        ("JoJo's Bizarre Adventure Vol. 1", "Araki, Hirohiko", "olid", "Manga - Adventure"),
        ("Rurouni Kenshin Vol. 1", "Watsuki, Nobuhiro", "olid", "Manga - Historical"),
        ("Yu Yu Hakusho Vol. 1", "Togashi, Yoshihiro", "olid", "Manga - Action"),
        ("Inuyasha Vol. 1", "Takahashi, Rumiko", "olid", "Manga - Fantasy"),
        ("Ranma 1/2 Vol. 1", "Takahashi, Rumiko", "olid", "Manga - Comedy"),
        ("Fruits Basket Vol. 1", "Takaya, Natsuki", "olid", "Manga - Romance"),
    ]
    
    # Combine all new books
    all_new = classics + american_lit + british + scifi_fantasy + mystery + horror + comics + manga
    
    # Add only books that don't exist yet
    for title, author, gid, category in all_new:
        if title not in existing_titles:
            book_type = "Comic" if "Comic" in category or "Manga" in category else "Novel"
            
            # Generate cover URL
            if gid == "olid":
                cover = "https://covers.openlibrary.org/b/id/default-M.jpg"
            elif gid == "gutenberg":
                cover = "https://www.gutenberg.org/files/default/cover.jpg"
            else:
                cover = f"https://www.gutenberg.org/cache/epub/{gid}/pg{gid}.cover.medium.jpg"
            
            new_books.append({
                "bookID": book_id,
                "title": title,
                "author": author,
                "isbn": f"ISBN-{book_id:04d}",
                "category": category,
                "copies": 5,
                "availableCopies": 5,
                "coverImage": cover,
                "type": book_type
            })
            
            existing_titles.add(title)
            book_id += 1
    
    # Add new books to existing
    data['books'].extend(new_books)
    
    # Save
    with open('library_data.json', 'w') as f:
        json.dump(data, f, indent=2)
    
    total = len(data['books'])
    novels = len([b for b in data['books'] if b['type'] == 'Novel'])
    comics = len([b for b in data['books'] if b['type'] == 'Comic'])
    
    print(f"✅ Added {len(new_books)} new unique books")
    print(f"📚 Total Books: {total}")
    print(f"📖 Novels: {novels}")
    print(f"🦸 Comics/Manga: {comics}")

if __name__ == "__main__":
    add_more_famous_books()

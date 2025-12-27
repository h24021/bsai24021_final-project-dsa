#!/usr/bin/env python3
"""
Add more diverse famous books to reach 1000+
"""

import json

def add_even_more_books():
    with open('library_data.json', 'r') as f:
        data = json.load(f)
    
    existing_books = data['books']
    max_id = max(book['bookID'] for book in existing_books)
    existing_titles = {book['title'] for book in existing_books}
    
    new_books = []
    book_id = max_id + 1
    
    # More Modern Classics
    modern = [
        ("To Kill a Mockingbird", "Lee, Harper", "modern", "Novel - Classic"),
        ("1984", "Orwell, George", "modern", "Novel - Dystopian"),
        ("Animal Farm", "Orwell, George", "modern", "Novel - Satire"),
        ("The Catcher in the Rye", "Salinger, J.D.", "modern", "Novel - Classic"),
        ("Lord of the Flies", "Golding, William", "modern", "Novel - Classic"),
        ("Brave New World", "Huxley, Aldous", "modern", "Novel - Dystopian"),
        ("Fahrenheit 451", "Bradbury, Ray", "modern", "Novel - Sci-Fi"),
        ("The Martian Chronicles", "Bradbury, Ray", "modern", "Novel - Sci-Fi"),
        ("Something Wicked This Way Comes", "Bradbury, Ray", "modern", "Novel - Fantasy"),
        ("I, Robot", "Asimov, Isaac", "modern", "Novel - Sci-Fi"),
        ("Foundation", "Asimov, Isaac", "modern", "Novel - Sci-Fi"),
        ("Dune", "Herbert, Frank", "modern", "Novel - Sci-Fi"),
        ("The Hobbit", "Tolkien, J.R.R.", "modern", "Novel - Fantasy"),
        ("The Fellowship of the Ring", "Tolkien, J.R.R.", "modern", "Novel - Fantasy"),
        ("The Two Towers", "Tolkien, J.R.R.", "modern", "Novel - Fantasy"),
        ("The Return of the King", "Tolkien, J.R.R.", "modern", "Novel - Fantasy"),
        ("The Silmarillion", "Tolkien, J.R.R.", "modern", "Novel - Fantasy"),
        ("A Wizard of Earthsea", "Le Guin, Ursula K.", "modern", "Novel - Fantasy"),
        ("The Left Hand of Darkness", "Le Guin, Ursula K.", "modern", "Novel - Sci-Fi"),
        ("The Dispossessed", "Le Guin, Ursula K.", "modern", "Novel - Sci-Fi"),
        ("Slaughterhouse-Five", "Vonnegut, Kurt", "modern", "Novel - Sci-Fi"),
        ("Cat's Cradle", "Vonnegut, Kurt", "modern", "Novel - Satire"),
        ("Breakfast of Champions", "Vonnegut, Kurt", "modern", "Novel - Satire"),
        ("The Handmaid's Tale", "Atwood, Margaret", "modern", "Novel - Dystopian"),
        ("Neuromancer", "Gibson, William", "modern", "Novel - Sci-Fi"),
        ("Snow Crash", "Stephenson, Neal", "modern", "Novel - Sci-Fi"),
        ("The Hitchhiker's Guide to the Galaxy", "Adams, Douglas", "modern", "Novel - Sci-Fi"),
        ("The Restaurant at the End of the Universe", "Adams, Douglas", "modern", "Novel - Sci-Fi"),
        ("Life, the Universe and Everything", "Adams, Douglas", "modern", "Novel - Sci-Fi"),
        ("So Long, and Thanks for All the Fish", "Adams, Douglas", "modern", "Novel - Sci-Fi"),
        ("Ender's Game", "Card, Orson Scott", "modern", "Novel - Sci-Fi"),
        ("Speaker for the Dead", "Card, Orson Scott", "modern", "Novel - Sci-Fi"),
        ("The Chronicles of Narnia: The Lion, the Witch and the Wardrobe", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("Prince Caspian", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("The Voyage of the Dawn Treader", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("The Silver Chair", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("The Horse and His Boy", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("The Magician's Nephew", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("The Last Battle", "Lewis, C.S.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Philosopher's Stone", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Chamber of Secrets", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Prisoner of Azkaban", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Goblet of Fire", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Order of the Phoenix", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Half-Blood Prince", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("Harry Potter and the Deathly Hallows", "Rowling, J.K.", "modern", "Novel - Fantasy"),
        ("A Game of Thrones", "Martin, George R.R.", "modern", "Novel - Fantasy"),
        ("A Clash of Kings", "Martin, George R.R.", "modern", "Novel - Fantasy"),
        ("A Storm of Swords", "Martin, George R.R.", "modern", "Novel - Fantasy"),
        ("A Feast for Crows", "Martin, George R.R.", "modern", "Novel - Fantasy"),
        ("A Dance with Dragons", "Martin, George R.R.", "modern", "Novel - Fantasy"),
    ]
    
    # Classic Mystery/Thriller
    thriller = [
        ("And Then There Were None", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("Murder on the Orient Express", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("The Murder of Roger Ackroyd", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("Death on the Nile", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("The ABC Murders", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("Evil Under the Sun", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("The Body in the Library", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("A Murder is Announced", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("The Secret Adversary", "Christie, Agatha", "gut", "Novel - Mystery"),
        ("The Man in the Brown Suit", "Christie, Agatha", "gut", "Novel - Thriller"),
    ]
    
    # More Historical Fiction
    historical = [
        ("The Last Days of Pompeii", "Bulwer-Lytton, Edward", "1840", "Novel - Historical"),
        ("Ben-Hur", "Wallace, Lew", "2145", "Novel - Historical"),
        ("Quo Vadis", "Sienkiewicz, Henryk", "2853", "Novel - Historical"),
        ("The Robe", "Douglas, Lloyd C.", "modern", "Novel - Historical"),
        ("I, Claudius", "Graves, Robert", "modern", "Novel - Historical"),
        ("Claudius the God", "Graves, Robert", "modern", "Novel - Historical"),
        ("The Name of the Rose", "Eco, Umberto", "modern", "Novel - Historical"),
        ("The Pillars of the Earth", "Follett, Ken", "modern", "Novel - Historical"),
        ("World Without End", "Follett, Ken", "modern", "Novel - Historical"),
    ]
    
    # Philosophical/Literary Fiction
    philosophical = [
        ("The Stranger", "Camus, Albert", "modern", "Novel - Philosophical"),
        ("The Plague", "Camus, Albert", "modern", "Novel - Philosophical"),
        ("The Fall", "Camus, Albert", "modern", "Novel - Philosophical"),
        ("Nausea", "Sartre, Jean-Paul", "modern", "Novel - Philosophical"),
        ("No Exit", "Sartre, Jean-Paul", "modern", "Novel - Philosophical"),
        ("The Trial", "Kafka, Franz", "modern", "Novel - Philosophical"),
        ("The Castle", "Kafka, Franz", "modern", "Novel - Philosophical"),
        ("The Metamorphosis", "Kafka, Franz", "modern", "Novel - Philosophical"),
        ("Siddhartha", "Hesse, Hermann", "modern", "Novel - Philosophical"),
        ("Steppenwolf", "Hesse, Hermann", "modern", "Novel - Philosophical"),
        ("The Glass Bead Game", "Hesse, Hermann", "modern", "Novel - Philosophical"),
        ("Thus Spoke Zarathustra", "Nietzsche, Friedrich", "modern", "Novel - Philosophical"),
    ]
    
    # More Marvel Comics
    marvel = [
        ("Amazing Spider-Man Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Fantastic Four Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("X-Men Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("The Avengers Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Thor Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Iron Man Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Captain America Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Daredevil Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Doctor Strange Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("The Incredible Hulk Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Silver Surfer Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("Black Panther Vol. 1", "Lee, Stan", "olid", "Comic - Superhero"),
        ("The Punisher Vol. 1", "Conway, Gerry", "olid", "Comic - Crime"),
        ("Wolverine Vol. 1", "Claremont, Chris", "olid", "Comic - Superhero"),
        ("Deadpool Vol. 1", "Nicieza, Fabian", "olid", "Comic - Superhero"),
        ("Ms. Marvel Vol. 1", "Wilson, G. Willow", "olid", "Comic - Superhero"),
        ("Moon Knight Vol. 1", "Moench, Doug", "olid", "Comic - Superhero"),
        ("Ghost Rider Vol. 1", "Friedrich, Gary", "olid", "Comic - Horror"),
        ("Blade Vol. 1", "Wolfman, Marv", "olid", "Comic - Horror"),
        ("The New Mutants Vol. 1", "Claremont, Chris", "olid", "Comic - Superhero"),
    ]
    
    # More DC Comics
    dc = [
        ("Wonder Woman Vol. 1", "Marston, William Moulton", "olid", "Comic - Superhero"),
        ("The Flash Vol. 1", "Broome, John", "olid", "Comic - Superhero"),
        ("Green Lantern Vol. 1", "Broome, John", "olid", "Comic - Superhero"),
        ("Aquaman Vol. 1", "Fradon, Ramona", "olid", "Comic - Superhero"),
        ("Teen Titans Vol. 1", "Haney, Bob", "olid", "Comic - Superhero"),
        ("Justice League of America Vol. 1", "Fox, Gardner", "olid", "Comic - Superhero"),
        ("Green Arrow Vol. 1", "Weisinger, Mort", "olid", "Comic - Superhero"),
        ("Hawkman Vol. 1", "Fox, Gardner", "olid", "Comic - Superhero"),
        ("The Spectre Vol. 1", "Binder, Otto", "olid", "Comic - Horror"),
        ("Constantine Vol. 1", "Delano, Jamie", "olid", "Comic - Horror"),
        ("Harley Quinn Vol. 1", "Dini, Paul", "olid", "Comic - Superhero"),
        ("Catwoman Vol. 1", "Brubaker, Ed", "olid", "Comic - Crime"),
        ("Batgirl Vol. 1", "Simone, Gail", "olid", "Comic - Superhero"),
        ("Nightwing Vol. 1", "Dixon, Chuck", "olid", "Comic - Superhero"),
        ("Red Hood Vol. 1", "Winick, Judd", "olid", "Comic - Superhero"),
        ("Suicide Squad Vol. 1", "Ostrander, John", "olid", "Comic - Crime"),
        ("Shazam Vol. 1", "Parker, Bill", "olid", "Comic - Superhero"),
        ("Black Adam Vol. 1", "Johns, Geoff", "olid", "Comic - Superhero"),
        ("Martian Manhunter Vol. 1", "Ostrander, John", "olid", "Comic - Superhero"),
        ("Cyborg Vol. 1", "Walker, David F.", "olid", "Comic - Superhero"),
    ]
    
    # More Manga Series
    more_manga = [
        ("Bleach Vol. 1", "Kubo, Tite", "olid", "Manga - Action"),
        ("Fairy Tail Vol. 1", "Mashima, Hiro", "olid", "Manga - Fantasy"),
        ("Black Clover Vol. 1", "Tabata, Yuki", "olid", "Manga - Fantasy"),
        ("My Hero Academia Vol. 1", "Horikoshi, Kohei", "olid", "Manga - Superhero"),
        ("Demon Slayer Vol. 1", "Gotouge, Koyoharu", "olid", "Manga - Action"),
        ("Tokyo Ghoul Vol. 1", "Ishida, Sui", "olid", "Manga - Horror"),
        ("Parasyte Vol. 1", "Iwaaki, Hitoshi", "olid", "Manga - Horror"),
        ("Uzumaki", "Ito, Junji", "olid", "Manga - Horror"),
        ("Tomie", "Ito, Junji", "olid", "Manga - Horror"),
        ("Gyo", "Ito, Junji", "olid", "Manga - Horror"),
        ("Promised Neverland Vol. 1", "Shirai, Kaiu", "olid", "Manga - Thriller"),
        ("Steins;Gate Vol. 1", "5pb", "olid", "Manga - Sci-Fi"),
        ("Mob Psycho 100 Vol. 1", "ONE", "olid", "Manga - Comedy"),
        ("Assassination Classroom Vol. 1", "Matsui, Yusei", "olid", "Manga - Comedy"),
        ("Food Wars Vol. 1", "Tsukuda, Yuto", "olid", "Manga - Comedy"),
        ("Haikyu!! Vol. 1", "Furudate, Haruichi", "olid", "Manga - Sports"),
        ("Kuroko's Basketball Vol. 1", "Fujimaki, Tadatoshi", "olid", "Manga - Sports"),
        ("Prince of Tennis Vol. 1", "Konomi, Takeshi", "olid", "Manga - Sports"),
        ("Blue Lock Vol. 1", "Kaneshiro, Muneyuki", "olid", "Manga - Sports"),
        ("Chainsaw Man Vol. 1", "Fujimoto, Tatsuki", "olid", "Manga - Horror"),
        ("Fire Force Vol. 1", "Ohkubo, Atsushi", "olid", "Manga - Action"),
        ("Soul Eater Vol. 1", "Ohkubo, Atsushi", "olid", "Manga - Fantasy"),
        ("Blue Exorcist Vol. 1", "Kato, Kazue", "olid", "Manga - Fantasy"),
        ("Noragami Vol. 1", "Adachitoka", "olid", "Manga - Fantasy"),
        ("Magi Vol. 1", "Ohtaka, Shinobu", "olid", "Manga - Fantasy"),
        ("The Seven Deadly Sins Vol. 1", "Suzuki, Nakaba", "olid", "Manga - Fantasy"),
        ("Dr. Stone Vol. 1", "Inagaki, Riichiro", "olid", "Manga - Sci-Fi"),
        ("The Quintessential Quintuplets Vol. 1", "Haruba, Negi", "olid", "Manga - Romance"),
        ("Kaguya-sama: Love is War Vol. 1", "Akasaka, Aka", "olid", "Manga - Romance"),
        ("Horimiya Vol. 1", "HERO", "olid", "Manga - Romance"),
    ]
    
    # Independent Comics
    indie = [
        ("Bone Vol. 1", "Smith, Jeff", "olid", "Comic - Fantasy"),
        ("Scott Pilgrim Vol. 1", "O'Malley, Bryan Lee", "olid", "Comic - Comedy"),
        ("Invincible Vol. 1", "Kirkman, Robert", "olid", "Comic - Superhero"),
        ("The Walking Dead Vol. 1", "Kirkman, Robert", "olid", "Comic - Horror"),
        ("Locke & Key Vol. 1", "Hill, Joe", "olid", "Comic - Horror"),
        ("American Born Chinese", "Yang, Gene Luen", "olid", "Comic - Drama"),
        ("Blankets", "Thompson, Craig", "olid", "Comic - Biography"),
        ("Habibi", "Thompson, Craig", "olid", "Comic - Drama"),
        ("Black Hole", "Burns, Charles", "olid", "Comic - Horror"),
        ("Palestine", "Sacco, Joe", "olid", "Comic - Journalism"),
        ("Safe Area Gorazde", "Sacco, Joe", "olid", "Comic - Journalism"),
    ]
    
    all_new = modern + thriller + historical + philosophical + marvel + dc + more_manga + indie
    
    for title, author, gid, category in all_new:
        if title not in existing_titles:
            book_type = "Comic" if "Comic" in category or "Manga" in category else "Novel"
            
            if gid == "olid":
                cover = "https://covers.openlibrary.org/b/id/default-M.jpg"
            elif gid == "modern" or gid == "gut":
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
    
    data['books'].extend(new_books)
    
    with open('library_data.json', 'w') as f:
        json.dump(data, f, indent=2)
    
    total = len(data['books'])
    novels = len([b for b in data['books'] if b['type'] == 'Novel'])
    comics = len([b for b in data['books'] if b['type'] == 'Comic'])
    
    print(f"✅ Added {len(new_books)} more unique books")
    print(f"📚 Total Books: {total}")
    print(f"📖 Novels: {novels}")
    print(f"🦸 Comics/Manga: {comics}")

if __name__ == "__main__":
    add_even_more_books()

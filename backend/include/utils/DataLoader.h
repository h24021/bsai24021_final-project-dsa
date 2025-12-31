#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include "../services/Library.h"

using namespace std;

class DataLoader {
public:
    static bool loadFromFile(Library& library, const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open " << filename << endl;
            return false;
        }

        string content((istreambuf_iterator<char>(file)),
                           istreambuf_iterator<char>());
        file.close();


        size_t booksPos = content.find("\"books\":");
        if (booksPos != string::npos) {
            parseBooks(library, content, booksPos);
        }

        size_t usersPos = content.find("\"users\":");
        if (usersPos != string::npos) {
            parseUsers(library, content, usersPos);
        }

        return true;
    }

private:
    static string extractValue(const string& json, const string& key, size_t startPos) {
        size_t keyPos = json.find("\"" + key + "\":", startPos);
        if (keyPos == string::npos) return "";
        
        size_t valueStart = json.find(":", keyPos) + 1;
        while (json[valueStart] == ' ') valueStart++;
        
        if (json[valueStart] == '"') {
            valueStart++;
            size_t valueEnd = json.find('"', valueStart);
            return json.substr(valueStart, valueEnd - valueStart);
        } else {
            size_t valueEnd = json.find_first_of(",}\n", valueStart);
            string num = json.substr(valueStart, valueEnd - valueStart);
            size_t first = num.find_first_not_of(" \t\n\r");
            size_t last = num.find_last_not_of(" \t\n\r");
            return (first == string::npos) ? "" : num.substr(first, last - first + 1);
        }
    }

    static int extractInt(const string& json, const string& key, size_t startPos) {
        string value = extractValue(json, key, startPos);
        return value.empty() ? 0 : stoi(value);
    }

    static void parseBooks(Library& library, const string& json, size_t startPos) {
        size_t pos = json.find('[', startPos);
        if (pos == string::npos) return;

        int count = 0;
        while (true) {
            size_t objStart = json.find('{', pos);
            if (objStart == string::npos) break;
            
            size_t objEnd = json.find('}', objStart);
            if (objEnd == string::npos) break;

            if (json.substr(objStart, objEnd - objStart).find("\"email\"") != string::npos) {
                break;
            }

            int id = extractInt(json, "bookID", objStart);
            string title = extractValue(json, "title", objStart);
            string author = extractValue(json, "author", objStart);
            string isbn = extractValue(json, "isbn", objStart);
            string category = extractValue(json, "category", objStart);
            int copies = extractInt(json, "copies", objStart);
            int availableCopies = extractInt(json, "availableCopies", objStart);
            // Try "cover" field first (new real covers), fallback to "coverImage"
            string coverImage = extractValue(json, "cover", objStart);
            if (coverImage.empty()) {
                coverImage = extractValue(json, "coverImage", objStart);
            }
            string type = extractValue(json, "type", objStart);

            if (id > 0 && !title.empty()) {
                library.addBook(Book(id, title, author, isbn, category, copies, availableCopies, coverImage, type));
                count++;
            }

            pos = objEnd + 1;
            

            size_t nextObj = json.find('{', pos);
            size_t usersSection = json.find("\"users\"", pos);
            if (usersSection != string::npos && usersSection < nextObj) {
                break;
            }
        }
        cout << "Loaded " << count << " books from file\n";
    }

    static void parseUsers(Library& library, const string& json, size_t startPos) {
        size_t pos = json.find('[', startPos);
        if (pos == string::npos) return;

        int count = 0;
        while (true) {
            size_t objStart = json.find('{', pos);
            if (objStart == string::npos) break;
            
            size_t objEnd = json.find('}', objStart);
            if (objEnd == string::npos) break;

            int id = extractInt(json, "id", objStart);
            string name = extractValue(json, "name", objStart);
            string email = extractValue(json, "email", objStart);
            string role = extractValue(json, "role", objStart);
            string username = extractValue(json, "username", objStart);
            string password = extractValue(json, "password", objStart);

            if (username.empty() && !email.empty()) {
                username = email.substr(0, email.find('@'));
            }
            if (password.empty()) {
                password = "password123";
            }

            if (id > 0 && !name.empty()) {
                library.addUser(User(id, username, name, email, password, role));
                count++;
            }

            pos = objEnd + 1;
            if (pos >= json.size()) break;
        }
        cout << "Loaded " << count << " users from file\n";
    }
};

#endif

#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include "../services/Library.h"

class DataLoader {
public:
    // Simple JSON parser for our specific format
    static bool loadFromFile(Library& library, const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return false;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();

        // Parse books
        size_t booksPos = content.find("\"books\":");
        if (booksPos != std::string::npos) {
            parseBooks(library, content, booksPos);
        }

        // Parse users
        size_t usersPos = content.find("\"users\":");
        if (usersPos != std::string::npos) {
            parseUsers(library, content, usersPos);
        }

        return true;
    }

private:
    static std::string extractValue(const std::string& json, const std::string& key, size_t startPos) {
        size_t keyPos = json.find("\"" + key + "\":", startPos);
        if (keyPos == std::string::npos) return "";
        
        size_t valueStart = json.find(":", keyPos) + 1;
        while (json[valueStart] == ' ') valueStart++;
        
        if (json[valueStart] == '"') {
            // String value
            valueStart++;
            size_t valueEnd = json.find('"', valueStart);
            return json.substr(valueStart, valueEnd - valueStart);
        } else {
            // Number value
            size_t valueEnd = json.find_first_of(",}\n", valueStart);
            std::string num = json.substr(valueStart, valueEnd - valueStart);
            // Trim whitespace
            size_t first = num.find_first_not_of(" \t\n\r");
            size_t last = num.find_last_not_of(" \t\n\r");
            return (first == std::string::npos) ? "" : num.substr(first, last - first + 1);
        }
    }

    static int extractInt(const std::string& json, const std::string& key, size_t startPos) {
        std::string value = extractValue(json, key, startPos);
        return value.empty() ? 0 : std::stoi(value);
    }

    static void parseBooks(Library& library, const std::string& json, size_t startPos) {
        size_t pos = json.find('[', startPos);
        if (pos == std::string::npos) return;

        int count = 0;
        while (true) {
            size_t objStart = json.find('{', pos);
            if (objStart == std::string::npos) break;
            
            size_t objEnd = json.find('}', objStart);
            if (objEnd == std::string::npos) break;

            // Check if we've moved to users section
            if (json.substr(objStart, objEnd - objStart).find("\"email\"") != std::string::npos) {
                break;
            }

            int id = extractInt(json, "id", objStart);
            std::string title = extractValue(json, "title", objStart);
            std::string author = extractValue(json, "author", objStart);
            std::string isbn = extractValue(json, "isbn", objStart);
            std::string category = extractValue(json, "category", objStart);
            int copies = extractInt(json, "copies", objStart);
            int availableCopies = extractInt(json, "availableCopies", objStart);

            if (id > 0 && !title.empty()) {
                library.addBook(Book(id, title, author, isbn, category, copies, availableCopies));
                count++;
            }

            pos = objEnd + 1;
            
            // Stop if we hit the users section
            size_t nextObj = json.find('{', pos);
            size_t usersSection = json.find("\"users\"", pos);
            if (usersSection != std::string::npos && usersSection < nextObj) {
                break;
            }
        }
        std::cout << "Loaded " << count << " books from file\n";
    }

    static void parseUsers(Library& library, const std::string& json, size_t startPos) {
        size_t pos = json.find('[', startPos);
        if (pos == std::string::npos) return;

        int count = 0;
        while (true) {
            size_t objStart = json.find('{', pos);
            if (objStart == std::string::npos) break;
            
            size_t objEnd = json.find('}', objStart);
            if (objEnd == std::string::npos) break;

            int id = extractInt(json, "id", objStart);
            std::string name = extractValue(json, "name", objStart);
            std::string email = extractValue(json, "email", objStart);
            std::string role = extractValue(json, "role", objStart);

            if (id > 0 && !name.empty()) {
                library.addUser(User(id, name, email, role));
                count++;
            }

            pos = objEnd + 1;
            if (pos >= json.size()) break;
        }
        std::cout << "Loaded " << count << " users from file\n";
    }
};

#endif

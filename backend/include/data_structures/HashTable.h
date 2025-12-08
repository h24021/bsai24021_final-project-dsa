#pragma once
#include <vector>
#include <list>
#include <functional>
#include <optional>
using namespace std;

template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry(K k, V v) : key(k), value(v) {}
    };
    
    vector<list<Entry>> table;
    int capacity;
    int size;
    hash<K> hashFunction;
    
    int getHash(const K& key) const {
        return hashFunction(key) % capacity;
    }
    
    void rehash() {
        vector<list<Entry>> oldTable = table;
        capacity *= 2;
        table.clear();
        table.resize(capacity);
        size = 0;
        
        for (auto& bucket : oldTable) {
            for (auto& entry : bucket) {
                insert(entry.key, entry.value);
            }
        }
    }

public:
    HashTable(int cap = 101) {
        capacity = cap;
        size = 0;
        table.resize(capacity);
    }
    
    void insert(const K& key, const V& value) {
        int index = getHash(key);

        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }

        table[index].push_back(Entry(key, value));
        size++;

        if ((double)size / capacity > 0.75) {
            rehash();
        }
    }
    
    optional<V> find(const K& key) const {
        int index = getHash(key);
        for (const auto& entry : table[index]) {
            if (entry.key == key) {
                return entry.value;
            }
        }
        return nullopt;
    }
    
    bool remove(const K& key) {
        int index = getHash(key);
        auto& bucket = table[index];
        
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                size--;
                return true;
            }
        }
        return false;
    }
    
    bool contains(const K& key) const {
        return find(key).has_value();
    }
    
    int getSize() const {
        return size;
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    void clear() {
        table.clear();
        table.resize(capacity);
        size = 0;
    }

    vector<V> getAllValues() const {
        vector<V> values;
        for (const auto& bucket : table) {
            for (const auto& entry : bucket) {
                values.push_back(entry.value);
            }
        }
        return values;
    }

    vector<K> getAllKeys() const {
        vector<K> keys;
        for (const auto& bucket : table) {
            for (const auto& entry : bucket) {
                keys.push_back(entry.key);
            }
        }
        return keys;
    }
};

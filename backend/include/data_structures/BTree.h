#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "../models/Book.h"

using namespace std;

// Forward declaration
template <typename T> class BTree;

// B-Tree Node
template <typename T>
class BTreeNode {
public:
    vector<T> keys;              // Keys stored in node
    vector<BTreeNode*> children; // Child pointers
    int t;                       // Minimum degree (minimum keys = t-1)
    bool isLeaf;                 // True if leaf node
    
    BTreeNode(int degree, bool leaf);
    ~BTreeNode();
    
    // Search for a key in subtree rooted at this node
    BTreeNode* search(const T& key, function<int(const T&, const T&)> compare);
    
    // Insert a key in non-full node
    void insertNonFull(const T& key, function<int(const T&, const T&)> compare);
    
    // Split child at index i
    void splitChild(int i, BTreeNode* child);
    
    // Traverse all keys in subtree
    void traverse(function<void(const T&)> visit);
    
    // Search by predicate (for flexible searching)
    void searchByPredicate(function<bool(const T&)> predicate, vector<T>& results);
    
    friend class BTree<T>;
};

// B-Tree
template <typename T>
class BTree {
private:
    BTreeNode<T>* root;
    int t; // Minimum degree
    function<int(const T&, const T&)> compareFunc;
    
public:
    // Constructor with custom comparison function
    BTree(int degree, function<int(const T&, const T&)> compare);
    ~BTree();
    
    // Insert a key
    void insert(const T& key);
    
    // Search for a key
    T* search(const T& key);
    
    // Traverse the tree
    void traverse(function<void(const T&)> visit);
    
    // Search by custom predicate
    vector<T> searchByPredicate(function<bool(const T&)> predicate);
    
    // Get all elements (in-order)
    vector<T> getAllElements();
    
    // Check if tree is empty
    bool isEmpty() const;
};

// ============== BTreeNode Implementation ==============

template <typename T>
BTreeNode<T>::BTreeNode(int degree, bool leaf) {
    t = degree;
    isLeaf = leaf;
    keys.reserve(2 * t - 1);
    if (!isLeaf) {
        children.reserve(2 * t);
    }
}

template <typename T>
BTreeNode<T>::~BTreeNode() {
    for (auto child : children) {
        delete child;
    }
}

template <typename T>
BTreeNode<T>* BTreeNode<T>::search(const T& key, function<int(const T&, const T&)> compare) {
    int i = 0;
    while (i < keys.size() && compare(key, keys[i]) > 0) {
        i++;
    }
    
    if (i < keys.size() && compare(keys[i], key) == 0) {
        return this;
    }
    
    if (isLeaf) {
        return nullptr;
    }
    
    return children[i]->search(key, compare);
}

template <typename T>
void BTreeNode<T>::insertNonFull(const T& key, function<int(const T&, const T&)> compare) {
    int i = keys.size() - 1;
    
    if (isLeaf) {
        keys.push_back(key);
        while (i >= 0 && compare(keys[i], key) > 0) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
    } else {
        while (i >= 0 && compare(keys[i], key) > 0) {
            i--;
        }
        i++;
        
        if (children[i]->keys.size() == 2 * t - 1) {
            splitChild(i, children[i]);
            if (compare(keys[i], key) < 0) {
                i++;
            }
        }
        children[i]->insertNonFull(key, compare);
    }
}

template <typename T>
void BTreeNode<T>::splitChild(int i, BTreeNode* child) {
    BTreeNode* newNode = new BTreeNode(child->t, child->isLeaf);
    int mid = t - 1;
    
    // Copy second half of keys to new node
    for (int j = 0; j < t - 1; j++) {
        newNode->keys.push_back(child->keys[mid + 1 + j]);
    }
    
    // Copy second half of children if not leaf
    if (!child->isLeaf) {
        for (int j = 0; j < t; j++) {
            newNode->children.push_back(child->children[mid + 1 + j]);
        }
        child->children.resize(t);
    }
    
    // Move middle key up to parent
    keys.insert(keys.begin() + i, child->keys[mid]);
    
    // Resize old child
    child->keys.resize(mid);
    
    // Insert new child pointer
    children.insert(children.begin() + i + 1, newNode);
}

template <typename T>
void BTreeNode<T>::traverse(function<void(const T&)> visit) {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!isLeaf) {
            children[i]->traverse(visit);
        }
        visit(keys[i]);
    }
    
    if (!isLeaf) {
        children[i]->traverse(visit);
    }
}

template <typename T>
void BTreeNode<T>::searchByPredicate(function<bool(const T&)> predicate, vector<T>& results) {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!isLeaf) {
            children[i]->searchByPredicate(predicate, results);
        }
        if (predicate(keys[i])) {
            results.push_back(keys[i]);
        }
    }
    
    if (!isLeaf) {
        children[i]->searchByPredicate(predicate, results);
    }
}

// ============== BTree Implementation ==============

template <typename T>
BTree<T>::BTree(int degree, function<int(const T&, const T&)> compare) {
    root = nullptr;
    t = degree;
    compareFunc = compare;
}

template <typename T>
BTree<T>::~BTree() {
    if (root) {
        delete root;
    }
}

template <typename T>
void BTree<T>::insert(const T& key) {
    if (root == nullptr) {
        root = new BTreeNode<T>(t, true);
        root->keys.push_back(key);
    } else {
        if (root->keys.size() == 2 * t - 1) {
            BTreeNode<T>* newRoot = new BTreeNode<T>(t, false);
            newRoot->children.push_back(root);
            newRoot->splitChild(0, root);
            
            int i = 0;
            if (compareFunc(newRoot->keys[0], key) < 0) {
                i++;
            }
            newRoot->children[i]->insertNonFull(key, compareFunc);
            
            root = newRoot;
        } else {
            root->insertNonFull(key, compareFunc);
        }
    }
}

template <typename T>
T* BTree<T>::search(const T& key) {
    if (root == nullptr) {
        return nullptr;
    }
    
    BTreeNode<T>* node = root->search(key, compareFunc);
    if (node == nullptr) {
        return nullptr;
    }
    
    for (auto& k : node->keys) {
        if (compareFunc(k, key) == 0) {
            return &k;
        }
    }
    return nullptr;
}

template <typename T>
void BTree<T>::traverse(function<void(const T&)> visit) {
    if (root != nullptr) {
        root->traverse(visit);
    }
}

template <typename T>
vector<T> BTree<T>::searchByPredicate(function<bool(const T&)> predicate) {
    vector<T> results;
    if (root != nullptr) {
        root->searchByPredicate(predicate, results);
    }
    return results;
}

template <typename T>
vector<T> BTree<T>::getAllElements() {
    vector<T> elements;
    traverse([&elements](const T& item) {
        elements.push_back(item);
    });
    return elements;
}

template <typename T>
bool BTree<T>::isEmpty() const {
    return root == nullptr;
}

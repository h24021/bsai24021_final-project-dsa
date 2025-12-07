#pragma once
#include <iostream>
using namespace std;

class User {
private:
    int userID;
    string name;
    string role;
    int borrowedBooks;

public:
    User();
    User(int id, string n, string r);

    void printUser();
    int getUserID();
};

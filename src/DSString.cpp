#include "DSString.h"
#include <cstring>  // Only used in implementation, not in the class itself
#include <iostream>

// Default constructor
DSString::DSString() {
    data = new char[1];
    data[0] = '\0';
    len = 0;
}

// Constructor from C-string
DSString::DSString(const char* str) {
    len = 0;
    while (str[len] != '\0') {
        len++;
    }
    
    data = new char[len + 1];
    
    // Copy characters
    for (int i = 0; i < len; i++) {
        data[i] = str[i];
    }
    data[len] = '\0';  // Null terminator
}

// Copy constructor
DSString::DSString(const DSString& source) {
    len = source.len;
    data = new char[len + 1];
    
    // Copy characters
    for (int i = 0; i < len; i++) {
        data[i] = source.data[i];
    }
    data[len] = '\0';  // Null terminator
}

// Destructor
DSString::~DSString() {
    delete[] data;
}

// Assignment operator
DSString& DSString::operator=(const DSString& source) {
    if (this != &source) {  // Self-assignment check
        delete[] data;  // Free existing memory
        
        len = source.len;
        data = new char[len + 1];
        
        // Copy characters
        for (int i = 0; i < len; i++) {
            data[i] = source.data[i];
        }
        data[len] = '\0';  // Null terminator
    }
    return *this;
}

// Length accessor
int DSString::length() const {
    return len;
}

// Bracket operators for non-const and const objects
char& DSString::operator[](int index) {
    return data[index];
}

char DSString::operator[](int index) const {
    return data[index];
}

// Comparison operators - implement the rest similarly
bool DSString::operator==(const DSString& rhs) const {
    if (len != rhs.len) {
        return false;
    }
    
    for (int i = 0; i < len; i++) {
        if (data[i] != rhs.data[i]) {
            return false;
        }
    }
    
    return true;
}

// Implement other comparison operators...

// String operations
DSString DSString::operator+(const DSString& rhs) const {
    // Create a buffer big enough for both strings plus null terminator
    char* buffer = new char[len + rhs.len + 1];
    
    // Copy first string
    for (int i = 0; i < len; i++) {
        buffer[i] = data[i];
    }
    
    // Copy second string
    for (int i = 0; i < rhs.len; i++) {
        buffer[len + i] = rhs.data[i];
    }
    
    buffer[len + rhs.len] = '\0';
    
    // Create result and clean up
    DSString result(buffer);
    delete[] buffer;
    
    return result;
}

// Implement other string operations...

// C-string conversion
const char* DSString::c_str() const {
    return data;
}

// Implement utility functions...
#ifndef DSSTRING_H
#define DSSTRING_H

class DSString {
private:
    char* data;
    int len;  // Length attribute for fast access

public:
    // Constructors
    DSString();
    DSString(const char* str);
    DSString(const DSString& source);  // Copy constructor

    // Destructor
    ~DSString();

    // Assignment operator
    DSString& operator=(const DSString& source);  // For rule of three

    // Accessor functions
    int length() const;
    char& operator[](int index);
    char operator[](int index) const;

    // Comparison operators
    bool operator==(const DSString& rhs) const;
    bool operator!=(const DSString& rhs) const;
    bool operator>(const DSString& rhs) const;
    bool operator<(const DSString& rhs) const;
    bool operator>=(const DSString& rhs) const;
    bool operator<=(const DSString& rhs) const;

    // String operations
    DSString operator+(const DSString& rhs) const;
    DSString& operator+=(const DSString& rhs);
    DSString substring(int start, int numChars) const;
    
    // C-string conversion
    const char* c_str() const;
    
    // Additional utility functions
    void toLowerCase();
    bool contains(const DSString& substr) const;
};

#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

const string BOOK_FILE = "books.txt";

class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;
    string dueDate;

    Book() : id(0), title(""), author(""), isIssued(false), dueDate("") {}

    string serialize() const {
        ostringstream oss;
        oss << id << "|" << title << "|" << author << "|" << isIssued << "|" << dueDate;
        return oss.str();
    }

    static Book deserialize(const string& record) {
        Book book;
        istringstream iss(record);
        string token;
        getline(iss, token, '|');
        book.id = stoi(token);
        getline(iss, book.title, '|');
        getline(iss, book.author, '|');
        getline(iss, token, '|');
        book.isIssued = (stoi(token) != 0);
        getline(iss, book.dueDate);
        return book;
    }
};

vector<Book> loadBooks() {
    vector<Book> books;
    ifstream inFile(BOOK_FILE);
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            books.push_back(Book::deserialize(line));
        }
    }
    inFile.close();
    return books;
}

void saveBooks(const vector<Book>& books) {
    ofstream outFile(BOOK_FILE);
    for (const Book &book : books) {
        outFile << book.serialize() << endl;
    }
    outFile.close();
}

void addBook() {
    vector<Book> books = loadBooks();
    Book newBook;
    newBook.id = books.empty() ? 1 : books.back().id + 1;
    
    cout << "Enter book title: ";
    getline(cin >> ws, newBook.title);
    cout << "Enter book author: ";
    getline(cin, newBook.author);
    
    newBook.isIssued = false;
    newBook.dueDate = "";
    books.push_back(newBook);
    
    saveBooks(books);
    cout << "Book added successfully! Book ID: " << newBook.id << endl;
}

void issueBook() {
    vector<Book> books = loadBooks();
    int bookId;
    cout << "Enter the book ID to issue: ";
    cin >> bookId;
    
    bool found = false;
    for (Book &book : books) {
        if (book.id == bookId) {
            found = true;
            if (book.isIssued) {
                cout << "This book is already issued! Due date: " << book.dueDate << endl;
            } else {
                cout << "Enter due date (e.g., 2025-05-01): ";
                cin >> book.dueDate;
                book.isIssued = true;
                cout << "Book issued successfully with due date: " << book.dueDate << endl;
            }
            break;
        }
    }
    if (!found)
        cout << "Book with ID " << bookId << " not found!" << endl;
    
    saveBooks(books);
}

void returnBook() {
    vector<Book> books = loadBooks();
    int bookId;
    cout << "Enter the book ID to return: ";
    cin >> bookId;
    
    bool found = false;
    for (Book &book : books) {
        if (book.id == bookId) {
            found = true;
            if (!book.isIssued) {
                cout << "This book was not issued." << endl;
            } else {
                book.isIssued = false;
                book.dueDate = "";
                cout << "Book returned successfully!" << endl;
            }
            break;
        }
    }
    if (!found)
        cout << "Book with ID " << bookId << " not found!" << endl;
    
    saveBooks(books);
}

void searchBook() {
    vector<Book> books = loadBooks();
    string keyword;
    cout << "Enter title keyword to search: ";
    getline(cin >> ws, keyword);
    
    bool found = false;
    for (const Book &book : books) {
        if (book.title.find(keyword) != string::npos) {
            cout << "ID: " << book.id << " | Title: " << book.title
                 << " | Author: " << book.author
                 << " | Issued: " << (book.isIssued ? "Yes" : "No");
            if (book.isIssued)
                cout << " | Due Date: " << book.dueDate;
            cout << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No books found with keyword '" << keyword << "'." << endl;
}

void listBooks() {
    vector<Book> books = loadBooks();
    cout << "Listing all books:" << endl;
    for (const Book &book : books) {
        cout << "ID: " << book.id << " | Title: " << book.title
             << " | Author: " << book.author
             << " | Issued: " << (book.isIssued ? "Yes" : "No");
        if (book.isIssued)
            cout << " | Due Date: " << book.dueDate;
        cout << endl;
    }
}

int main() {
    int choice;
    while (true) {
        cout << "\nLibrary Management System Menu:" << endl;
        cout << "1. Add new book" << endl;
        cout << "2. Issue book" << endl;
        cout << "3. Return book" << endl;
        cout << "4. Search book" << endl;
        cout << "5. List all books" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        cin.ignore();
        
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                issueBook();
                break;
            case 3:
                returnBook();
                break;
            case 4:
                searchBook();
                break;
            case 5:
                listBooks();
                break;
            case 6:
                cout << "Exiting the system. Goodbye!" << endl;
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return 0;
}

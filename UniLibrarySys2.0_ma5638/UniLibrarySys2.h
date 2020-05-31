//============================================================================
// Name         : UniLibrarySys.cpp
// Author       : Muhammad Ali
// NetID		: ma5638
// Version      : 2.0
// Date Created : 06-05-2020
// Date Modified: 11-05-2020 
// Copyright    : All rights are reserved
// Description  : Library System: header file
//============================================================================
// 								DATA STRUCTURE:
// Books are in a singly-linked list because adding a book to the front is efficient
// Shelves are stored in an array as their indices can correspond to their shelf number. 
// 			- Direct access improves the efficiency.
// Bookcases are stored in an array to use direct access to improve efficiency
//			- Letters are converted to integers to correspond to indices
// Hash table is used for creating the search catalogue
//			- Chosen because it is the most efficient search mechanism
//			- Separate chaining has been used to handle collisions
//		
// ===========================================================================
// DISCLAIMER: ADHERE TO THE FOLLOWING RULES:
// 1. BOOKCASES SHOULD BE NAMED AFTER LETTERS AND IN EXACT SEQUENTIAL ORDER
//			- This is because of the charToIndex function in class Library that relies on the letters being sequential
// 		e.g. Bookcases A,B,C,D,E,..., Y, Z will be accepted
//			 Bookcases G,H,I,.. will also be accepted
//			 HOWEVER, Bookcase A, C, D, G will not be accepted
//			 Z,S,P will also not be accepted
// 2. POLYNOMIAL_HASH_A value of 9, constant used in hash code, was chosen by tests from 1 to 10 and was found most efficient
//			- Avoid changing the value to 1
//			- DO NOT change the value to 0
//============================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string.h>
#include <bits/stdc++.h> 
#include <list>



using namespace std;
// Global Variables

const int NUMBOOKCASES = 3;
const int NUMSHELVES = 5;
const int NUM_BOOKS_IN_SHELF = 10;
const int NUMFIELDS = 5;		//Number of categories for the books in csv files including title, author etc.
const int HASH_CAPACITY = 100;		// capacity of hash table to store books
const int POLYNOMIAL_HASH_A = 9;	// value of coefficient of x when finding hashcode
const char name_bookcase[NUMBOOKCASES] = {'A','B','C'};		// Book cases name
const string PASSWORD = "NYUAD@2020";

//===================================================

class Book{			// Node for Shelf (implemented as linked list in Shelf class)
	friend class Shelf;
	friend class Bookcase;
	friend class Library;
private:
	string title;
	string author;
	int yearOfPublication;
	string ISBN;
	string identifier;			// location of book

	Book* next;
public:
	Book(): next(NULL){}	// O(1)
	Book(string title, string author, string ISBN, int yearOfPublication, string identifier): 
		title(title), author(author), yearOfPublication(yearOfPublication),ISBN(ISBN), identifier(identifier)
		{}	// O(1)
};


//===================================================
class Shelf{
	friend class Bookcase;
	friend class Library;
private:
	int capacity;		// Value = 10;
	int size;
	string identifier;			// A1, B4 etc.

	Book* head;

public:
	Shelf();					//O(1)
	Shelf(string identifier);	//O(1)
	~Shelf();					//O(n), n is the number of books in shelf
	bool empty() const;			//O(1)
	void addBook(Book book);	//O(n)
	void removeBook(int index) throw (out_of_range)	;	//O(n)//
};


class Bookcase{
	friend class Library;
private:
	Shelf* s[NUMSHELVES];	//An array/list of 5 shelves the bookcases must have, as an array
	ifstream bookCase;
	ofstream editBooks;		//for updating files
	string identifier;
	// string copy;

public:
	~Bookcase();	//O(n), n is the number of books
	Bookcase(string letter);	// O(n), n is the number of books in the bookcase
	void updateFiles();		// O(n), n is the number of books in the bookcase
	// helper functions
	void addBook(string book);// O(n) where n is the number of copies being added
	void getFields(string book, string bookAttrib[]);	// O(n), n = NUMFIELDS
};


//===================================================
// Hashmap used for searches. Title and author act as the primary keys. 
// Identifier differentiates between same books at different locations

// Hash Map adapted/updated from:
//============================================================================
// Name         : lab12.2.cpp
// Author       : Khalid Mengal
// Version      : 1.0
// Date Created : 25-04-2020
// Date Modified: 03-04-2020 
// Copyright    : All rights are reserved
// Description  : HashMap using chaining
//============================================================================
class HashMap
{
private:
	// Hash Node
	class BookHashNode
	{
		friend class HashMap;
	private:

		string title;
		string author;
		string identifier;
	public:
		// O(1)
		BookHashNode(string title, string author, string identifier): 
		title(title), author(author), identifier(identifier){}

	};

private:
	// there are approximately 80 books in the default files
	// (current chosen) capacity of Hashmap is 100
	// load factor is ~ 80/100=0.8
	// so separate chaining was used
	list<BookHashNode> *buckets;		// Array of lists of type Hash Nodes
	int size;					    //Current Size of HashMap
	int capacity;				    // Total Capacity of HashMap
public:
	HashMap();
	~HashMap();
	unsigned long hashCode(const string title, const string author);	// O(n), n is the combined length of title and author // Hash function combines title and author
	void insert(const string title, const string author, const string identifier);	//O(1)
	string search(const string title, const string author);			//O(1) expected
	int getSize();	//O(1)
	void clear();	// O(n), n is size of hashmap

};



class Library{
	friend int main();

private:
	Bookcase* lib[NUMBOOKCASES];		// Bookcases in the library
	HashMap searchCatalogue;
public:
	Library();
	
	
	// Operations
	void addBook();		// O(1)
	void addBook(Book b, string location, int copies);	// O(n), n is the number of copies of the book to be added
	void removeBook();	// O(1)
	void removeBook(string location);	// O(1)
	void moveBook();	//O(n)
	void getNumOfBooksInShelf();	// O(1)
	void getNumOfBookcases();	//O(1)
	void searchBook();	//O(1)
	// Other Functions
	bool verifyUser();	// O(1)
	void refreshCatalogue();	//O(n), n is the total number of books in the whole library
	void updateFiles(); //-> update everything or only the changes	//O(N), N is the number of bookshelves
	void systemLoop();
	// helper functions

	
	int charToIndex(char letter){			// O(1) // convert bookcase character to its index number
		return letter % NUMBOOKCASES;
	}

	// O(n), n is the size of the string
	string toLower(string s){	// convert string to lowercase
		string result = "";
		for(int i = 0; i < s.size(); i++){
			int ascii = s[i];
			if(ascii>90 || ascii<65)
				result+=(ascii);
			else
				result+=(ascii+32);
		}
		return result;
	}

	// O(1)
	string firstName(string s){		// to get the first name of the author
		int index_space = s.find(' ');
		return s.substr(0, index_space);

	}


};


//============================================================================
// Name         : UniLibrarySys.cpp
// Author       : Muhammad Ali
// NetID		: ma5638
// Version      : 2.0
// Date Created : 06-05-2020
// Date Modified: 11-05-2020 
// Copyright    : All rights are reserved
// Description  : Library System: cpp file
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


#include "UniLibrarySys2.h"


//O(1)
Shelf::Shelf(){
	identifier = "";
	size = 0;
	capacity = NUM_BOOKS_IN_SHELF;
	head = NULL;
}

//O(1)
Shelf::Shelf(string identifier){
	this->identifier = identifier;
	size = 0;
	capacity = NUM_BOOKS_IN_SHELF;
	head = NULL;
}

// O(n), n is the number of elements/books in shelf
Shelf::~Shelf(){
	while(!empty()){
		removeBook(1);
	}
}

// O(1)
bool Shelf::empty() const{
	if(head == NULL){
		return true;
	}
	return false;
}




// Insertion: O(1)
// Update Book Identifiers: O(n)
// Overall function: O(n)
void Shelf::addBook(Book book){
	string name = identifier + ".1";
	Book *tmp = this->head;
	this->head = new Book(book.title,book.author,book.ISBN,book.yearOfPublication,name);
	this->head->next = tmp;


	// Changing location identifiers of the rest of the books
	Book* current = head->next;
	int count = 2;

	while(current!=NULL){

		current->identifier = identifier + "." + to_string(count);
		current = current->next;
		count++;

	}
}

// Remove + update all identifiers: O(n)
void Shelf::removeBook(int index) throw (out_of_range)		// index will start from 1
{
	if(index>size)
		throw out_of_range("Book not Found!");	// throw exception if out of range

	size--;
	int currIndex = 1;					// index will start from 1
	Book* current = head;
	if(currIndex == index){
		// code to remove current
		Book* temp = current;
		head = head->next;
		delete temp;
		current = head;
	}


	if(head==NULL){		// if there was are no books after removed book, stop proceeding
		return;
	}

	// Start to change the identifiers of the following books
	current->identifier = identifier + "." + to_string(currIndex);
	currIndex++;

	while(current->next != NULL){
		if(currIndex == index){
			// code to remove current->next

			Book* temp = current->next;
			current->next = current->next->next;
			delete temp;

		}

		if(current->next != NULL)
		{

			current->next->identifier = identifier + "." + to_string(currIndex);

			// cout << current->next->identifier<< endl;
		} 
		else
			return;		// stop function if the next node is NULL
		currIndex++;
		current = current->next;
	}
}

//===================================================



// Creating bookcases with their letter identifier
// O(n), n is the number of books
Bookcase::Bookcase(string letter)
{ 
	//Making the Shelves
	for(int i = 0; i < NUMSHELVES; i++){
		string name = letter + to_string(i+1);
		s[i] = new Shelf(name);
	}
	//Adding the Books
	identifier = letter;
	bookCase.open("Bookcases/Bookcase" + letter + ".csv");
	// copy = "";
	string myText = "";
	getline(bookCase,myText);
	while(getline(bookCase, myText)){
		addBook(myText);
		// copy = copy + myText + "\n";
		// function for adding books to shelves
	}
	bookCase.close();
}

// O(n), n is the number of shelves
Bookcase::~Bookcase(){
	
	for(int i = 0; i < NUMSHELVES; i++){
		delete s[i];
	}

}

// O(n),n = NUMFIELDS
void Bookcase::getFields(string book, string bookAttrib[]){
	istringstream ss(book);
	string field;	
	string subField = "";		// Add multiple seprate fields together if they have commas in them
	bool inQuotes = false;		// To check if the current field is in quotes
	int count = 0;

	// If the first letter of the field is a string, then a special
	// set of operations must be performed to ensure that 
	// the commas in between double quotes are not seperated
	// Dividing Fields
	while(getline(ss,field,',')){
		
		if(field[0] == '\"'){ 
		inQuotes = true;  							// If the field starts with quotes, no commas should be accepted
		field = field.substr(1,field.size()-1); 	
		}	
		if(field[field.size()-1] == '\"' && inQuotes)	// If the field ends with quotes, accept commas from then on
		{
			subField += field.substr(0,field.size()-1);		// remove quotes out of name
			inQuotes = false;
			field = subField;
			subField = "";
		}


		if(!inQuotes)	// If there are no quotes, add the field to the bookAtrribute array
		{
			bookAttrib[count] = field;
			count ++;
		}
		else{
			subField += field;		//If the field is in quotes, add it to the subfield that will be later added to the array
		}
	}
}


// O(n), n is the number of copies of the book to add
void Bookcase::addBook(string book)			// function to initialize bookcase by putting books in shelves
{
	string bookAttrib[NUMFIELDS];
	getFields(book, bookAttrib);
	// Code to add to shelf
	for(int i = 0; i <NUMSHELVES; i++){
		if(s[i]->capacity - s[i]->size >= stoi(bookAttrib[4])){
			for(int j = 0; j < stoi(bookAttrib[4]); j++){
				s[i]->addBook(Book(bookAttrib[0], bookAttrib[1], bookAttrib[2], stoi(bookAttrib[3]), ""));
				// "" was put as the location as a placeholder. The addBook function in the Shelf will determine the identifier
				s[i]->size++;
			}
			break;
		}
	}


}

// save changes to actual bookcases files
// O(n), n is the number of books in bookcase
void Bookcase::updateFiles(){
	editBooks.open("Bookcases/Bookcase" + identifier + ".csv");	// editBooks and bookCase
	string newText = "Title,Author,ISBN,Year,Number_of_copies\n";	// first line
	string line;

	string name = "";
	int copies = 0;
	Book* current;
	
	for(int i = 0; i < NUMSHELVES; i++){
		// get each book from here
		current = s[i]->head;
			
			while(current!=NULL){
				if(current->title == name){
					copies++;
				} else{
					
					name = current -> title;
					copies = 1;
				}
				if(current->next == NULL || current->next->title != name){
					line = name + "," + current->author + "," + current->ISBN + "," + to_string(current->yearOfPublication) + "," + to_string(copies) + "\n";
					newText += line;
				}
				current = current -> next;
			}
		name = "";
		copies = 0;
	}
	newText.erase(newText.size()-1,1); // erase the last \n character
	// cout << newText << endl;
	editBooks << newText;
	editBooks.close();

}

//===================================================
// Hashmap used for searches. Title and author act as the primary keys. 
//Identifier differentiates between same books at different locations

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


HashMap::HashMap()
{
	buckets = new list<BookHashNode>[capacity];			//Create an array of size capacity. Each locaiton of Array is a list/chain
	this->capacity = HASH_CAPACITY;						// global variable to decide capacity
	this->size = 0;
}

//=====================================================
HashMap::~HashMap()
{
	for(int i=0; i<capacity; i++)
		buckets[i].clear();
	
	delete[] this->buckets;
}
//======================================================

// O(title+author), or O(n) where n is the combined length of the title and the author
unsigned long HashMap::hashCode(const string title, const string author)
{
	string key = title + author;		// add title and author together in a string for key
	unsigned long code = 0;
	for(int i = 0; i < key.length();i++)
		code += (unsigned long)(key[i]*pow(POLYNOMIAL_HASH_A,i));	// after experimenting with values 2 to 10, 
																	// Polynomial hash value a of 9 was found to have the least collisions (0 collisions)
	return code;
}
//================================================

// repetitions (i.e. same title, author AND identifier) will be handled before insertion
// Insertion: O(1)
void HashMap::insert(const string title, const string author, const string identifier)
{
	//TODO
	unsigned long index = hashCode(title, author) % capacity;
	if(buckets[index].size()>=1){
		cout << "Collision occured at index: " << index << endl;
		cout << "New size of bucket after insertion: " << buckets[index].size()+1 << endl;
	}
	buckets[index].push_back(BookHashNode(title, author, identifier));
	size++;
}
//===================================================
// search: O(1) on average
string HashMap::search(const string title, const string author)
{
	unsigned long index = hashCode(title, author) % capacity;
	for(list<BookHashNode>::iterator it = buckets[index].begin(); it != buckets[index].end();it++)
	{
		if((*it).title == title && (*it).author == author){
			return (*it).identifier;
		}
	}
	return "N/A";
}
//=====================================================
//O(1)
int HashMap::getSize(){return size;}
//=====================================================
// O(n), n is the size of hashmap
void HashMap::clear(){
	for(int i=0; i<capacity; i++)
		buckets[i].clear();
	size = 0;
}
//=====================================================



Library::Library(){
	char letter;
	int count = 0;
	while(count<NUMBOOKCASES) {	
		letter = name_bookcase[count];			// a global variable
		lib[charToIndex(letter)] = new Bookcase(string(1, letter));
		count++;
	}
}



void Library::systemLoop(){	// Keep asking the user what to do
	int choice = 0;
	do{
		try{
			choice = 0;
			refreshCatalogue();
			cout << endl<< endl<<"Welcome to NYUAD's Library System" << endl;
			cout << "=================================" << endl << endl;
			cout << "What do you wish to do?" << endl << endl;
			cout << "1. Search for a Book" << endl;
			cout << "2. Add a book" << endl;
			cout << "3. Remove a book" << endl;
			cout << "4. Move a book" << endl;
			cout << "5. Get the number of Books in a Shelf" << endl;
			cout << "6. Get the number of Bookcases" << endl;
			cout << "7. Exit" << endl;

			//inputting
			string choiceStr;
			getline(cin, choiceStr);
			choice = stoi(choiceStr);
			if(choice > 7 || choice <1){
				cout << "Invalid input" << endl;
				cout << "Please make sure you are entering an integer from 1 to 7" << endl;

			}
			switch(choice)
			{
			case 1: searchBook();
					break;
			case 2: 
					if(verifyUser()){
						addBook();
					}
					break;
			case 3: 
					if(verifyUser()){
						removeBook();
					}
					break;
			case 4: 
					if(verifyUser()){
						moveBook();
					}
					break;
			case 5: 
					if(verifyUser()){
						getNumOfBooksInShelf();
					}
					break;
			case 6: 
					if(verifyUser()){
						getNumOfBookcases();
					}
					break;


			}
			updateFiles();			// make sure files are up to date
		} catch(exception& e){
			cout << "Exception: " << e.what() << endl;
		}
	} while(choice != 7);

}

// Creates the vector search catalogue

// O(n), n is all the books in all the bookcases
void Library::refreshCatalogue(){
	// loop through bookcases
	//loop through shelves
	// add every book to vector
	// with exception of books already in 
	searchCatalogue.clear();
	string location = "";
	string name = "";
	string author = "";
	int copies = 1;
	Book* current;

	for(int i = 0; i < NUMBOOKCASES; i++){
		for(int j = 0; j < NUMSHELVES; j++){
			current = lib[i]->s[j]->head;
			
			while(current!=NULL){
				if(current->title == name){
					// inSeries = true;
					copies++;
				} else{
					
					name = current -> title;
					author = current -> author;
					location = current -> identifier;
					copies = 1;
				}
				if(current->next == NULL || current->next->title != name){
					searchCatalogue.insert(toLower(name), toLower(firstName(author)), location);
				}
				current = current -> next;
			}
			name = "";
			copies = 0;
			location = "";

		}
	}
}

// Search for book
// O(1)
void Library::searchBook(){
	// string findBook = "introdUCtion to Mathematical Machine Theory";
	string title = "";
	string author = "";

	cout << "Please enter the title of the book: ";
	getline(cin, title);
	

	cout << "Please enter the first name of the author of the book: ";
	getline(cin, author);

	title = toLower(title);
	author = toLower(firstName(author));

	string location = searchCatalogue.search(title, author); /////////

	if(location=="N/A"){
		cout << "================" << endl;
		cout << "     Error!     " << endl;
		cout << "Book Not Found!" << endl;
		cout << "================" << endl;
	}
	else{
		cout << "Book found!" << endl;
		cout << "Location: " << location << endl;
	}
}


// Adding A New Book
// O(1)
void Library::addBook(){
	try{
		string title;
		string author;
		string yearOfPublication;
		string ISBN;
		int copies;
		string copiesStr;

		string location;
		string Bookcase;
		string Shelf;
		cout << "Enter the:" << endl;

		cout << "Title: ";
		getline(cin, title);

		cout << endl << "Author: ";
		getline(cin, author);

		cout << endl << "yearOfPublication: ";
		getline(cin, yearOfPublication);

		cout << endl << "ISBN: ";
		getline(cin, ISBN);

	
		cout << endl << "Copies: " << flush;
		getline(cin,copiesStr);
		copies = stoi(copiesStr);
	


		cout << " Now please enter the shelf you wish to add the book to: (e.g. A2, C3)" << endl;
		getline(cin, location);

		addBook(Book(title, author, ISBN, stoi(yearOfPublication), location),location, copies);


	}	catch(exception& e){
		cout << "=================================" << endl;
		cout << "Error: " << e.what() << endl;
		return;
	}

}


// Adding A Book given a Book object and location/identifier
//O(n), n is the number of copies of the book to be added
void Library::addBook(Book b, string location, int copies){
	try{
		char BookcaseID = location[0];
		int shelfNum = (((int)location[1])-48) -1;
		if(shelfNum>=NUMSHELVES || shelfNum<0)
			throw out_of_range("Shelf number does not exist");
		Shelf* s = lib[charToIndex(BookcaseID)]->s[shelfNum];

		if(s->capacity - s->size >= copies){
			for(int k = 0; k < copies; k++){
				s->addBook(b);
				s->size++;
			}
		} else{
			cout << "Shelf is full" << endl;
		}

	}	catch(exception& e){
		cout << "=================================" << endl;
		cout << "Error: " << e.what() << endl;
		return;
	}

}

// Remove a book
// O(1)
void Library::removeBook(){
		string location = "";
		cout << "Please enter the book identifier that you would like to remove: (A1.3, B2.1 etc.)" << flush;
		getline(cin, location);
		removeBook(location);
}

// O(1), 
void Library::removeBook(string location){
	try{
		char BookcaseID = location[0];
		int shelfNum = (((int)location[1])-48) -1;
		int bookNum = stoi(location.substr(3));
		if(shelfNum>=NUMSHELVES || shelfNum<0)
			throw out_of_range("Shelf number does not exist");

		Shelf* s = lib[charToIndex(BookcaseID)]->s[shelfNum];

		s->removeBook(bookNum);

		// int index = 1;		// remove book function counts from 1
		// Book* current;
		// current = s->head;

		// while(current!=NULL){

		// 	if(current->identifier == location){
		// 		s->removeBook(index);
		// 		cout <<"Removed!" << endl;
		// 		return;
		// 	}
		// 	index++;
		// 	current = current->next;
		// }
		// throw out_of_range("Book Not Found!");
		cout <<"Removed!" << endl;

	} catch(exception& e){
		cout << "=================================" << endl;
		cout << "Error: " << e.what() << endl;
	}
}


// Move A Book


// O(n), n is the number of books in the shelf
void Library::moveBook(){
	try{
		string location = "";
		string target = "";
		cout << "Please enter the book identifier that you would like to remove: (A1.3, B2.1 etc.)" << flush;
		getline(cin, location);

		char BookcaseID = location[0];
		int shelfNum = (((int)location[1])-48) -1;
		Shelf* s = lib[charToIndex(BookcaseID)]->s[shelfNum];

		if(shelfNum>=NUMSHELVES || shelfNum<0)
			throw out_of_range("Shelf number does not exist");


		int index = 1;		// remove book function counts from 1
		Book* current;
		current = s->head;
		while(current!=NULL){
			if(current->identifier == location){
				cout << "Which shelf would you like to move this book?" << flush;
				getline(cin, target);
				addBook(*current, target,1);	// add 1 copy
				s->removeBook(index);
				
				return;
			}
			index++;
			current = current->next;
		}

	}	catch(exception& e){
		cout << "=================================" << endl;
		cout << "Error: " << e.what() << endl;
		return;
	}
}


// Get number of books in a certain shelf
// O(1)
void Library::getNumOfBooksInShelf(){
	try{
		string location = "";
		cout << "Please enter which shelf you would like to check: (A1, B2 etc.)" << flush;
		getline(cin, location);

		char BookcaseID = location[0];
		int shelfNum = (((int)location[1])-48) -1;		//-1 to change shelfNum to index
		// cout << BookcaseID << shelfNum << endl;
		Shelf* s = lib[charToIndex(BookcaseID)]->s[shelfNum];

		if(shelfNum>=NUMSHELVES || shelfNum<0)
			throw out_of_range("Shelf number does not exist");

		cout << "There are " << s->size << " book(s) in shelf " << location << endl;

	}	catch(exception& e){
		cout << "=================================" << endl;
		cout << "Error: " << e.what() << endl;
		return;
	}
}

// Get number of bookcases
//O(1)
void Library::getNumOfBookcases(){
	cout << "There are: " << NUMBOOKCASES << " bookcases" << endl;
}


//======================================================
// O(N), N is NUMBOOKCASES
void Library::updateFiles()
{
	for(int i = 0; i < NUMBOOKCASES; i++){
		lib[i]->updateFiles();
	}
}


// Asks to verify
// O(1)
bool Library::verifyUser(){
	string attempt;
	cout << "This option can only be done by librarians." << endl;
	cout << "Please enter in the password: " << flush;
	getline(cin, attempt);

	if(attempt == PASSWORD){
		cout << endl << "Access Granted!" << endl;
		return true;
	} else{
		cout << endl << "Access Denied!" << endl;
		return false;
	}
}
//===================================================
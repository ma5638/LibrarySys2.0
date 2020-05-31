//============================================================================
// Name         : main.cpp
// Author       : Muhammad Ali
// NetID		: ma5638
// Version      : 2.0
// Date Created : 06-05-2020
// Date Modified: 11-05-2020 
// Copyright    : All rights are reserved
// Description  : Library System: main file
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


int main(){
	Library l;
	l.systemLoop();
}
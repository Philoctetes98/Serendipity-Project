#pragma once
#ifndef MAINMENU13_H
#define MAINMENU13_H
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
using namespace std;

//The inventory file
const char fileName[] = "Inventory.dat";

//Main Menu function prototypes
void cashier();
void InvMenu();
void reports();

//InvMenu function prototypes
void lookUpBook();
void addBook();
void editBook();
void deleteBook();

//Report function prototypes
void repListing();
void repWholesale();
void repRetail();
void repQty();
void repCost();
void repAge();

//Book Info function prototype
void BookInfo(char[], char[], char[], char[], char[],
	int, double, double);

//Capitalize each character in the string function prototype
void strUpper(char*);

//array sizes
const int SIZE = 20;
const int TITLESIZE = 51;
const int ISBNSIZE = 14;
const int AUTHORSIZE = 31;
const int PUBLISHERSIZE = 31;
const int DATEADDEDSIZE = 11;


//BookData class declaration 
class BookData
{
private:
	char bookTitle[TITLESIZE], isbn[ISBNSIZE],
		author[AUTHORSIZE], publisher[PUBLISHERSIZE], dateAdded[DATEADDEDSIZE];
	int QtyonHand;
	double wholesale, retail;
public:

	//Constructor
	BookData();
	//setter functions
	void setTitle(char*);
	void setISBN(char*);
	void setAuthor(char*);
	void setPublisher(char*);
	void setDateAdded(char*);
	void setQty(int);
	void setWholesale(double);
	void setRetail(double);

	//Check for empty array for books
	int isEmpty(BookData&);

	//Deletes the book from inventory
	void removeBook(BookData&, int);             

	//getter functions
	char*   getTitle();
	char*   getISBN();
	char*   getAuthor();
	char*   getPub();
	char*   getDateAdded();
	int     getQty();
	double  getWholesale();
	double  getRetail();
};

#endif // !MAINMENU_H





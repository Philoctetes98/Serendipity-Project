//Implementation file for the BookData class

#include "MainMenu13.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;


//Default constructor sets the title to an empty string
BookData::BookData()
{
	strcpy_s(bookTitle, "\0");
}

//Copies the title from the bookTitle member
void BookData::setTitle(char* title)
{
	strcpy_s(bookTitle, title);
}

//Copies the isbn from the ISBN member
void BookData::setISBN(char* newISBN)
{
	strcpy_s(isbn, newISBN);
}

//Copies the author from the author member
void BookData::setAuthor(char* newAuthor)
{
	strcpy_s(author, newAuthor);
}

//Copies the publisher from the publisher member
void BookData::setPublisher(char* newPublisher)
{
	strcpy_s(publisher, newPublisher);
}

//Copies the date added from the date added member
void BookData::setDateAdded(char* newDate)
{
	strcpy_s(dateAdded, newDate);
}

//Copies the quantity held from the quantity on hand member
void BookData::setQty(int newQuantity)
{
	QtyonHand = newQuantity;
}

//Copies the wholesale price from the wholesale member
void BookData::setWholesale(double newWholesale)
{
	wholesale = newWholesale;
}

//Copies the retail price from the retail member
void BookData::setRetail(double newRetail)
{
	retail = newRetail;
}

//Checks if there is an empty book array
int BookData::isEmpty(BookData &book)
{
	if (strcmp(book.getTitle(), "\0") == 0)
	{
		return 1;       //if this is true
	}
	else
	{
		return 0;       //if this is false
	}
}

//This allows for the deletion of a book
void BookData::removeBook(BookData &theBook, int subscript)
{
	theBook.bookTitle[0] = 0;
	theBook.isbn[0] = 0;
}

//This returns the bookTitle 
char* BookData::getTitle()
{
	return bookTitle;
}

//This returns the isbn number
char* BookData::getISBN()
{
	return isbn;
}

//This returns the author
char* BookData::getAuthor()
{
	return author;
}

//This returns the publisher
char* BookData::getPub()
{
	return publisher;
}

//This returns the date added
char* BookData::getDateAdded()
{
	return dateAdded;
}

//This returns the quantity of books on hand
int BookData::getQty()
{
	return QtyonHand;
}

//This returns the wholesale cost of the book
double BookData::getWholesale()
{
	return wholesale;
}

//This returns the retail price of the book
double BookData::getRetail()
{
	return retail;
}
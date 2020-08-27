//This is the main menu (single) file for the Serendipity Project
#include <iostream>
#include "MainMenu13.h"
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
#include <iomanip>
using namespace std;

//Defines an instance of the BookData class
BookData book[SIZE];

//Create the inventory file
fstream invFile;

//Define important variables for data fields
char Isbn[ISBNSIZE], Title[TITLESIZE], Author[AUTHORSIZE], Publisher[PUBLISHERSIZE],
DateAdded[DATEADDEDSIZE];
int quantityOnHand;
double wholesalePrice, retailPrice;

//string upper case function which accepts a pointer to a string argument
// and converts every character in the string to an uppercase letter
void strUpper(char* str)
{
	while (*str != 0)
	{
		*str = toupper(*str);
		*str++;
	}
}

//The Main Menu function
int main()
{
	//open the InvFile
	invFile.open(fileName, ios::in | ios::binary);

	//check if the file failed to open
	if (invFile.fail())
	{
		invFile.open(fileName, ios::out | ios::binary);
		invFile.close();
		invFile.clear();
		cout << "You have created the Inventory file but first you have to add books!\n" << endl;
		cout << "Press enter to continue......";
		addBook();
	}
	//if the file opens
	else
	{
		int counter = 0;
		while (!invFile.eof() && counter < SIZE)
		{
			invFile.seekg(counter * sizeof(book), ios::beg);
			invFile.read(reinterpret_cast<char *> (&book), sizeof(book));
			book[counter];
			counter++;
		}
		invFile.close();
		invFile.clear();
	}

	//The choice variable 
	int choice = 0;

	//The Main Menu
	while (choice != 4)
	{
		//Greeting
		cout << "******************************************\n";
		cout << "\t\t\t Serendipity Booksellers\n";
		cout << "\t\t\t\t Main Menu\n";
		cout << "******************************************\n";

		//rest of the menu
		cout << "1. Cashier Module\n";
		cout << "2. Inventory Database Module\n";
		cout << "3. Report Module\n";
		cout << "4. Exit\n";

		//Asks the user to enter a choice between 1 and 4
		cout << "Please enter a choice between 1 and 4: ";
		cin >> choice;

		//input validation 
		while (choice < 1 || choice > 4)
		{
			cout << "Please choose a value between 1 and 4: ";
			cin >> choice;
		}

		//Determines which module the user can operate based on the choice
		switch (choice)
		{
		case 1:
			cashier();
			break;
		case 2:
			InvMenu();
			break;
		case 3:
			reports();
			break;
		case 4:
			cout << "You have chosen to exit the program....\n";

			//writes the data to the inventory file
			invFile.open(fileName, ios::out | ios::binary);
			for (int record = 0; record < SIZE; record++)
			{
				invFile.seekp(record * sizeof(book), ios::beg);
				invFile.write(reinterpret_cast<char *> (&book), sizeof(book));
			}
			invFile.close();
			invFile.clear();
			break;
		}
	}

	system("pause");
	return 0;
}

//Book Info function
void BookInfo(char isbn[], char bookTitle[], char author[],
	char publisher[], char dateAdded[], int QtyonHand, double wholesale, double retail)
{
	//Greeting
	cout << "\t\t\t Serendipity Booksellers\n";
	cout << "\t\t\t\tBook Information\n";
	cout << "ISBN: " << isbn << endl;     //isbn number
	cout << "Title: " << bookTitle << endl; //title
	cout << "Author: " << author << endl;   //author
	cout << "Publisher: " << publisher << endl; //publisher
	cout << "Date Added: " << dateAdded << endl;  //date added
	cout << "Quantity-On-Hand: " << QtyonHand << endl; //quantity on hand
	cout << "Wholesale Cost: " << wholesale << endl;  //wholesale cost of the book
	cout << "Retail Price: " << retail << endl;      //retail price of the book
}

//Cashier function
void cashier()
{
	const double taxRate = 0.06; //The tax rate added to the subtotal
	char isbnNum[SIZE][ISBNSIZE]; //isbn number
	char transDate[DATEADDEDSIZE]; // The date of purchase
	char titleOfBook[SIZE][TITLESIZE]; //title of the book
	int QtyOfBook[SIZE]; //quantity of book(s) sold
	double subTotal[SIZE]; //Subtotal
	double bookPrice[SIZE]; //price of book(s)
	double salesOrderTax = 0; //accumulator will be used for the price calculation
	double runningTotal = 0; //accumulator for the subtotal which will be used for
							 //the price calculation
	double OrderTotal = 0; //accumulator which will be used for the total price
	int validQty;       //quantity of books validation 
	char choice = 'Y'; //the choice for another transaction 
	int counter = 0;  //counter variable
	int subQty = 0;  //the value that holds the quantity of books in memory

	//This goes on as long as the user wants to process another transaction
	while (toupper(choice) == 'Y')
	{
		//Greeting
		cout << "\t\t\tSerendipity Booksellers\n";
		cout << "\t\t\t Cashier Module\n";

		//The Date of the transaction
		cin.ignore();
		cout << "Date: ";
		cin.getline(transDate, DATEADDEDSIZE);

		//Retrieve the ISBN Number
		cout << "ISBN #: ";
		cin.getline(isbnNum[counter], ISBNSIZE);
		strUpper(isbnNum[counter]);

		//This goes through all of the books in the array to use the isbn number to 
		// search for the title and the price of the book as well as other data fields
		for (int row = 0; row < SIZE; row++)
		{
			//this makes sure the isbn entered matches the one in the inventory
			if (strcmp(isbnNum[counter], book[row].getTitle()) == 0)
			{
				cout << "Title: " << book[row].getTitle() << endl;  //The title of the book
				cout << fixed << showpoint << setprecision(2);
				cout << "Price: " << book[row].getRetail() << endl;   //The price of the book

				//keeps track of the book title and price in the inventory
				strcpy_s(titleOfBook[counter], book[row].getTitle());
				bookPrice[counter] = book[row].getRetail();

				//the quantity validation value. Makes sure the validation value
				//is equal to the number of books on hand as recorded in inventory
				validQty = book[row].getQty();

				//This is if there are no more books in inventory
				if (validQty == 0)
				{
					cout << "We are currently out of stock! \n";
					return;
				}

				//Enter the quantity of books
				cout << "Quantity of books to be purchased: ";
				cin >> QtyOfBook[counter];

				//input validation
				while (QtyOfBook[counter] < 1 || QtyOfBook[counter] > validQty)
				{
					//no negative amount of books
					if (QtyOfBook[counter] < 1)
						cout << "This is an invalid value. \n";
					else
						cout << "Sorry, there is an insufficient amount in inventory. \n";

					//Prompts the user to once more enter an acceptable quantity amount
					cout << "Please enter the quantity of books on hand: ";
					cin >> QtyOfBook[counter];
				}

				//subtracts the amount of copies purchased from the inventory total
				subQty = book[row].getQty();
				subQty -= QtyOfBook[counter];
				counter++;

				//asks the user if they would like to enter another transaction
				cout << "Would you like to add another transaction? ";
				cin >> choice;
				cout << endl;
				break;
			}
			//This goes to the last element of the array and the isbn number
			//is still not found
			else if (row == SIZE - 1)
			{
				cout << "The ISBN number has not been found.\n";
				cout << "Would you like to enter the ISBN number again? ";
				cin >> choice;
			}
		}
	}
	//Displays and calculates the prices
	if (counter > 0)
	{
		//POS display screen
		cout << "Serendipity Booksellers\n";
		cout << "Date: " << transDate << endl;
		cout << "Qty\tISBN\t\t Title \t\t\t\tPrice\t\t\t\t Total\n";
		cout << "__________________________________________________________________";
		cout << "\n\n\n";

		//calculates the prices
		for (int index = 0; index < counter; index++)
		{
			//Calculates the subtotal
			subTotal[index] = (bookPrice[index] * QtyOfBook[index]);
			//calculates the total with tax
			salesOrderTax += (taxRate*subTotal[index]);
			//adds up all of the book subtotals to get the final sub price
			runningTotal += subTotal[index];
			//This is the total price plus tax
			OrderTotal += (runningTotal + salesOrderTax);

			cout << QtyOfBook[index] << "\t"; //displays quantity
			cout << left << setw(14) << isbnNum[index] << "\t";        //displays the isbn number
			cout << left << setw(26) << titleOfBook[index] << "\t$";        //displays the book title
			cout << fixed << showpoint << right << setprecision(2);
			cout << setw(6) << bookPrice[index] << "\t$";      //displays the price
			cout << setw(6) << subTotal[index] << "\n\n\n";            //displays the subtotal
		}
		//Calculated Totals
		cout << "\t\tSubtotal\t\t\t\t\t\t$ ";
		cout << setw(6) << runningTotal << "\n";
		cout << "\t\tTax\t\t\t\t\t\t\t$ ";
		cout << setw(6) << salesOrderTax << "\n";
		cout << "\t\tTotal\t\t\t\t\t$ ";
		cout << setw(6) << OrderTotal << "\n\n";
		cout << "Thank you for shopping at Serendipity. Have a great day!\n";
	}
}


//Inventory Database function
void InvMenu()
{
	//The choice variable
	int choice = 0;

	//The menu
	while (choice != 5)
	{
		//Greeting
		cout << "\t\t\tSerendipity Booksellers\n";
		cout << "\t\t\t Inventory Database\n";

		//rest of the menu
		cout << "1. Look up a book\n";
		cout << "2. Add a book\n";
		cout << "3. Edit a book's record\n";
		cout << "4. Delete a book's record\n";
		cout << "5. Return to the Main Menu\n";

		//prompts the user to enter their choice
		cout << "Enter Your Choice: ";
		cin >> choice;

		//input validation 
		while (choice < 1 || choice > 5)
		{
			cout << "Please choose a value between 1 and 5: ";
			cin >> choice;
		}

		//displays the choice's output
		switch (choice)
		{
		case 1:
			lookUpBook();
			break;
		case 2:
			addBook();
			break;
		case 3:
			editBook();
			break;
		case 4:
			deleteBook();
			break;
		case 5:
			cout << "You have chosen to exit the Inventory Menu\n";
			break;
		}
	}
}

void reports()
{
	//This is the choice variable
	int choice = 0;


	//The Menu
	while (choice != 7)
	{
		//Greeting
		cout << "\t\t\t Serendipity Booksellers\n";
		cout << "\t\t\t\t Reports\n";

		//rest of the menu
		cout << "1. Inventory Listing\n";
		cout << "2. Inventory Wholesale Value\n";
		cout << "3. Inventory Retail Value\n";
		cout << "4. Listing by Quantity\n";
		cout << "5. Listing by Cost\n";
		cout << "6. Listing by Age\n";
		cout << "7. Return to the Main Menu\n";

		//Prompts the user to enter their choice
		cout << "Enter your choice: ";
		cin >> choice;

		//input validation 
		while (choice < 1 || choice > 7)
		{
			cout << "Please choose a value between 1 and 7: ";
			cin >> choice;
		}

		//displays the choice's output
		switch (choice)
		{
		case 1:
			repListing();
			break;
		case 2:
			repWholesale();
			break;
		case 3:
			repRetail();
			break;
		case 4:
			repQty();
			break;
		case 5:
			repCost();
			break;
		case 6:
			repAge();
			break;
		case 7:
			cout << "You have chosen to leave the Reports Menu\n";
			break;
		}

	}
}

//function for book look up in the InvMenu function
void lookUpBook()
{
	char choice = ' ';
	char searchTitle[TITLESIZE];             //This is the title which the user inputs
	cin.ignore();
	// prompts the user to enter the title to look up
	cout << "Please enter the title of the book you are searching for: ";
	cin.getline(searchTitle, TITLESIZE);

	//capitalizes the title search that the user puts in
	strUpper(searchTitle);

	int row;

	//loop which goes through the characters to compare if they were found in the book
	for (row = 0; row < SIZE; row++)
	{
		//This indicates as a possible match
		if (strstr(book[row].getTitle(), searchTitle))
		{
			cout << "Did you mean: " << book[row].getTitle() << endl;
			cout << "Yes(Y/y) or No(N/n)? ";
			cin >> choice;

			//input validation
			while (toupper(choice) != 'Y' && toupper(choice) != 'N')
			{
				cout << "Please enter a valid choice (Y/y) or (N/n): ";
				cout << "Did you mean: " << book[row].getTitle() << endl;
				cout << "Yes(Y/y) or No(N/n)? ";
				cin >> choice;
			}

			//if the entry is a match with the title
			if (toupper(choice) == 'Y')
			{
				//Lists the information for the title since, if it is found,
				//the information is kept in the inventory
				BookInfo(book[row].getISBN(), book[row].getTitle(), book[row].getAuthor(),
					book[row].getPub(), book[row].getDateAdded(),
					book[row].getQty(), book[row].getWholesale(), book[row].getRetail());
				break;
			}
		}
	}
	//The book hasn't been found so this message notifies the user of that fact
	if (row > SIZE - 1)
		cout << "No possible matches have been found. \n";
}

//function for adding a book in the InvMenu function
void addBook()
{
	//If there is an empty string within the element that is chosen
	for (int row = 0; row < SIZE; row++)
	{
		//First book with element 0
		if (book[row].isEmpty(book[row]))
		{
			//This allows for the user to enter all the information for the book to be
		//added
			cin.ignore();
			cout << "Enter the Title: ";        //Title
			cin.getline(Title, TITLESIZE);
			strUpper(Title);
			book[row].setTitle(Title);

			cout << "Enter the ISBN #: ";       //Isbn Number
			cin.getline(Isbn, ISBNSIZE);
			strUpper(Isbn);
			book[row].setISBN(Isbn);

			cout << "Enter the Author's Name: "; //Name of the author
			cin.getline(Author, AUTHORSIZE);
			strUpper(Author);
			book[row].setAuthor(Author);

			cout << "Enter the Publisher's Name: ";   //Name of the publisher
			cin.getline(Publisher, PUBLISHERSIZE);
			strUpper(Publisher);
			book[row].setPublisher(Publisher);

			cout << "Enter the date when the book was added to the inventory: "; //Date Added
			cin.getline(DateAdded, DATEADDEDSIZE);
			book[row].setDateAdded(DateAdded);

			cout << "Enter the quantity of the book being added: ";    //Quantity of said book added
			cin >> quantityOnHand;
			book[row].setQty(quantityOnHand);

			cout << "Enter the wholesale cost of the book: "; //Wholesale cost of the book
			cin >> wholesalePrice;
			book[row].setWholesale(wholesalePrice);

			cout << "Enter the retail price of the book: ";  //Retail price of the book
			cin >> retailPrice;
			book[row].setRetail(retailPrice);

			//Message which lets the user know that the book has been added to the inventory
			cout << "The book has been added on record. " << endl;
			break;
		}

		//No more books can be added
		if (row > SIZE - 1)
			cout << "No more books may be added to the inventory. \n";
	}

}

//function for book look up in the InvMenu function
void editBook()
{
	//Title which the user wants to edit the data of
	char editTitle[TITLESIZE];
	//choice of which data field the user wants to edit
	char choice;
	//menu choice
	int menuChoice;

	//prompts the user to enter the title of the book whose data is to be edited
	cin.ignore();
	cout << "Please enter the title of the book: ";
	cin.getline(editTitle, TITLESIZE);

	//capitalize the entry
	strUpper(editTitle);

	//loop which goes through all of the book titles in the array to see if there 
	//is a match
	for (int row = 0; row < SIZE; row++)
	{
		//If there is a possible match
		if (strstr(book[row].getTitle(), editTitle))
		{
			cout << "Did you mean: " << book[row].getTitle() << endl;
			cout << "Yes (Y/y) or No(N/n)? ";
			cin >> choice;

			//input validation
			while (toupper(choice) != 'Y' && toupper(choice) != 'N')
			{
				cout << "Please enter a valid choice.\n";
				cout << "Did you mean: " << book[row].getTitle() << endl;
				cout << "Yes (Y/y) or No(N/n)? ";
				cin >> choice;
			}

			//if the entry is a match with the title
			if (toupper(choice) == 'Y')
			{
				//Displays the data of the title that is found
				BookInfo(book[row].getISBN(), book[row].getTitle(), book[row].getAuthor(),
					book[row].getPub(), book[row].getDateAdded(),
					book[row].getQty(), book[row].getWholesale(), book[row].getRetail());

				do
				{
					//This lists the data fields of the book that can be edited
					cout << "These are the fields that can be edited\n";
					cout << "1. ISBN # \n";   //ISBN
					cout << "2. Title of the Book \n";  //Book Title
					cout << "3. Author \n";        //Author
					cout << "4. Publisher \n";       //Publisher
					cout << "5. The date added \n";   //Date Added
					cout << "6. The quantity on hand \n";  //Quantity on Hand
					cout << "7. The wholesale price \n";   //The Wholesale Price 
					cout << "8. The retail price \n";    //The Retail Price
					cout << "9. Exit \n";         //Allows for the user to exit this menu
					//Prompts the user to enter their choice
					cout << "Enter your choice: ";
					cin >> menuChoice;

					//input validation
					while (menuChoice < 1 || menuChoice > 9)
					{
						cout << "Please enter a choice between 1 and 9: ";
						cin >> menuChoice;
					}

					//This lists the current data then prompts the user to enter a new data
					//field for replacement
					switch (menuChoice)
					{
					case 1:
						cin.ignore();
						//Displays the current ISBN#
						cout << "Current Isbn#: " << book[row].getISBN() << endl;
						//Allows for the user to replace the current ISBN#
						cin.ignore();
						cout << "Please enter the new Isbn #: ";
						cin.getline(Isbn, ISBNSIZE);
						strUpper(Isbn);
						book[row].setISBN(Isbn);
						break;
					case 2:
						cin.ignore();
						//Displays the current ISBN#
						cout << "Current Book Title: " << book[row].getTitle() << endl;
						//Allows for the user to replace the current ISBN#
						cin.ignore();
						cout << "Please enter the new Book Title: ";
						strUpper(Title);
						cin.getline(Title, TITLESIZE);
						book[row].setTitle(Title);
						break;
					case 3:
						//Displays the current ISBN#
						cout << "Current Author: " << book[row].getAuthor() << endl;
						//Allows for the user to replace the current ISBN#
						cin.ignore();
						cout << "Please enter the new Author: ";
						strUpper(Author);
						cin.getline(Author, AUTHORSIZE);
						book[row].setAuthor(Author);
						break;
					case 4:
						cin.ignore();
						//Displays the current ISBN#
						cout << "Current Publisher: " << book[row].getPub() << endl;
						//Allows for the user to replace the current ISBN#
						cout << "Please enter the new Publisher: ";
						strUpper(Publisher);
						cin.getline(Publisher, PUBLISHERSIZE);
						book[row].setPublisher(Publisher);
						break;
					case 5:
						cin.ignore();
						//Displays the current ISBN#
						cout << "This is the current 'Date Added': " << book[row].getDateAdded() << "\n";
						//Allows for the user to replace the current ISBN#
						cout << "Please enter the new 'Date Added': ";
						cin.getline(DateAdded, DATEADDEDSIZE);
						book[row].setDateAdded(DateAdded);
						break;
					case 6:
						cin.ignore();
						//Displays the current ISBN#
						cout << "This is the current 'Quantity on Hand': " << book[row].getQty() << endl;
						//Allows for the user to replace the current ISBN#
						cin.ignore();
						cout << "Please enter the new 'Quantity on Hand': ";
						cin >> quantityOnHand;
						book[row].setQty(quantityOnHand);
						break;
					case 7:
						//Displays the current ISBN#
						cout << "This is the current wholesale price: " << book[row].getWholesale() << "\n";
						//Allows for the user to replace the current ISBN#
						cin.ignore();
						cout << "Please enter the new wholesale price: ";
						cin >> wholesalePrice;
						book[row].setWholesale(wholesalePrice);
						break;
					case 8:
						cin.ignore();
						//Displays the current ISBN#
						cout << "This is the current retail price: " << book[row].getRetail() << "\n";
						//Allows for the user to replace the current ISBN#
						cin.ignore();
						cout << "Please enter the new retail price: ";
						cin >> retailPrice;
						book[row].setRetail(retailPrice);
						break;
					}
				} while (menuChoice != 9);
				break;
			}
		}
		//This message lets the user know that the title entered is not in the 
		//inventory 
		if (row > SIZE - 1)
			cout << "No more possible matches have been found. \n";
	}
}

//function for deleting a book in the InvMenu function
void deleteBook()
{
	//This is the title the user enters which they would like to delete
	char deleteTitle[TITLESIZE];

	//The choice if the user found a match
	char choice;

	//The choice if the user wants to delete the title or not
	char deleteChoice;

	//prompts the user to enter the title they may want to enter
	cin.ignore();
	cout << "Please enter the book title you would like to search for: ";
	cin.getline(deleteTitle, TITLESIZE);

	//capitalize the entry
	strUpper(deleteTitle);

	//counter for the for loop
	int row;

	//loop which goes through all of the book titles in the array to see if there 
	//is a match
	for (row = 0; row < SIZE; row++)
	{
		//If a possible match is found
		if (strstr(book[row].getTitle(), deleteTitle))
		{
			cout << "Did you mean: " << book[row].getTitle() << endl;
			cout << "Yes(Y/y) or No(N/n)? ";
			cin >> choice;

			//input validation
			while (toupper(choice) != 'Y' && toupper(choice) != 'N')
			{
				cout << "Please enter a valid choice.\n";
				cout << "Did you mean: " << book[row].getTitle() << endl;
				cout << "Yes(Y/y) or No(N/n)? ";
				cin >> choice;
			}

			//if the entry matches the title
			if (toupper(choice) == 'Y')
				//If the book is found then the user sees the information of the book and
			  //is asked to make sure if they want to delete the book's record from the
			  //inventory
			{
				//Displays the information of the book
				BookInfo(book[row].getISBN(), book[row].getTitle(), book[row].getAuthor(),
					book[row].getPub(), book[row].getDateAdded(),
					book[row].getQty(), book[row].getWholesale(), book[row].getRetail());

				cin.ignore();
				//Asks the user one more time if they are absolutely certain that they
				//want to delete the book
				cout << "Are you sure you would like to delete this book's data from ";
				cout << "the inventory? ";
				cin >> deleteChoice;

				//input validation
				while (toupper(choice) != 'Y' && toupper(choice) != 'N')
				{
					cout << "Please enter a valid choice.\n";
					cout << "Did you mean: " << book[row].getTitle() << endl;
					cout << "Yes(Y/y) or No(N/n)? ";
					cin >> choice;
				}

				//If the user does want to delete the book, then it will be done
				if (toupper(deleteChoice) == 'Y')
				{
					//This deletes the book through the removeBook method
					book[row].removeBook(book[row], row);

					//This sends a confirmation message to the user to let them know that
					//the book has been deleted
					cout << "This book has been deleted from the record. \n";
				}
				else
					cout << "Record has not been deleted.\n";
				break;
			}
		}
	}
	//If there are no matches found, then this is a message that lets the user know
	   //that the book is not in the inventory
	if (row > SIZE - 1)
		cout << "No possible matches have been found\n";
}

//function for the inventory listing in the Reports function
void repListing()
{
	//null character
	char end[] = { '\0' };
	//Today's date
	char date[DATEADDEDSIZE];
	//prompts the user to enter the date
	cout << "Please enter today's date: ";
	cin.ignore();
	cin.getline(date, DATEADDEDSIZE);

	//Greeting which includes today's date
	cout << "\n\n\t\t Serendipity Booksellers Inventory Report\n";
	cout << "Date: \t\t" << date << endl;
	cout << "\t_______________________________________________________";

	//prints the inventory report on the screen
	for (int index = 0; index < SIZE; index++)
	{
		if (strcmp(book[index].getISBN(), end) != 0) //no empty strings
		{
			cout << "\n_________________________________________________________";
			cout << "\n\nTitle:\t\t\t" << book[index].getTitle();
			cout << "\nISBN:\t\t\t" << book[index].getISBN();
			cout << "\nAuthor:\t\t\t" << book[index].getAuthor();
			cout << "\nPublisher:\t\t" << book[index].getPub();
			cout << "\nDate Added:\t\t" << book[index].getDateAdded();
			//maintains the prices having two decimal points
			cout << fixed << showpoint << left << setprecision(2) << endl;
			cout << "\nQuantity on Hand:\t" << book[index].getQty();
			cout << "\nWholesale Price: \t\t" << book[index].getWholesale();
			cout << "\nRetail Price: \t\t" << book[index].getRetail();
		}
	}
	cout << endl;

	//display the ending of the report
	cout << "End of Inventory Report..." << endl;
	//Pause the report on the screen
	cout << "Please press any key to continue to the next screen: ";
	cin.ignore();
	cin.get();
}
//function for the wholesale value in the Reports function
void repWholesale()
{
	//null character
	char end[] = { '\0' };
	//Today's date
	char date[DATEADDEDSIZE];
	//prompts the user to enter the date
	cout << "Please enter today's date: ";
	cin.ignore();
	cin.getline(date, DATEADDEDSIZE);

	//data which will be used to find the total wholesale value
	double itemSubTotal = 0;
	double subTotal = 0;

	//Greeting which includes today's date
	cout << "\n\n\t\t Serendipity Booksellers Inventory Wholesale Report\n";
	cout << "Date: \t\t" << date << endl;
	cout << "\t____________________________________________________";

	//prints the inventory wholesale report on the screen
	for (int index = 0; index < SIZE; index++)
	{
		if (strcmp(book[index].getISBN(), end) != 0) //no empty strings
		{
			cout << "\n_________________________________________________________";
			cout << "\n\nTitle:\t\t\t" << book[index].getTitle();
			cout << "\nISBN:\t\t\t" << book[index].getISBN();
			//maintains the prices having two decimal points
			cout << fixed << showpoint << left << setprecision(2) << endl;
			cout << "\nQuantity on Hand:\t" << book[index].getQty();
			cout << "\nWholesale Price: \t$" << book[index].getWholesale();

			//Find the total wholesale value
			itemSubTotal += (book[index].getQty() * book[index].getWholesale());
			subTotal += itemSubTotal;
		}
	}

	//Display the total wholesale value
	cout << "\n\nTotal Wholesale Value of the items is: \t$" << subTotal << endl;

	//display the ending of the report
	cout << "End of Inventory Wholesale Report..." << endl;
	//Pause the report on the screen
	cout << "Please press any key to continue to the next screen: ";
	cin.ignore();
	cin.get();
}
//function for the inventory retail value in the Reports function
void repRetail()
{
	//null character
	char end[] = { '\0' };
	//Today's date
	char date[DATEADDEDSIZE];
	//prompts the user to enter the date
	cout << "Please enter today's date: ";
	cin.ignore();
	cin.getline(date, DATEADDEDSIZE);

	//Greeting which includes today's date
	cout << "\n\n\t\t Serendipity Booksellers Inventory Retail Report\n";
	cout << "Date: \t\t" << date << endl;
	cout << "\t____________________________________________________";

	//data used to find the total retail value of the items
	double itemSubTotal = 0;
	double subTotal = 0;

	//prints the inventory retail report on the screen
	for (int index = 0; index < SIZE; index++)
	{
		if (strcmp(book[index].getISBN(), end) != 0) //no empty strings
		{
			cout << "\n_________________________________________________________";
			cout << "\n\nTitle:\t\t\t" << book[index].getTitle();
			cout << "\nISBN:\t\t\t" << book[index].getISBN();
			//maintains the prices having two decimal points
			cout << fixed << showpoint << left << setprecision(2) << endl;
			cout << "\nQuantity on Hand:\t" << book[index].getQty();
			cout << "\nRetail Price: \t$" << book[index].getRetail();

			//Find the retail value
			itemSubTotal += (book[index].getQty() * book[index].getRetail());
			subTotal += itemSubTotal;
		}
	}
	//display the retail value of the items
	cout << "\n\n\nThe Total Retail Value of the items is: \t\t" << subTotal << endl;
	//display the ending of the report
	cout << "End of Inventory Retail Report..." << endl;
	//Pause the report on the screen
	cout << "Please press any key to continue to the next screen: ";
	cin.ignore();
	cin.get();
}
//function for the quantity listing value in the Reports function
void repQty()
{
	int id[SIZE]; //Original subscript tracking array
	int* idPtr[SIZE]; // pointers to tracking array
	int* QtyPtr[SIZE]; //array of pointers to quantity on hand array
	int bookQty[SIZE]; //array of book object accessor values

	for (int index = 0; index < SIZE; index++)
	{
		id[index] = index;   //sets the subscript tracking array to each individual subscript
		idPtr[index] = &id[index];  //sets the pointers to tracking array to the
										  //address of the subscript tracking array
		bookQty[index] = book[index].getQty();
		QtyPtr[index] = &bookQty[index]; 
	}

	int maxElement;  //max subscript value
	int* tempId;     //points to a temporary value which will be used
	int* maxValue;   //points to the max value within the qty on hand array
	//selection sort algorithm which will arrange these items by their QTY on Hand
	// in descending order
	for (int start = 0; start < SIZE - 1; start++)
	{
		maxElement = start;
		maxValue = QtyPtr[start];
		tempId = idPtr[start];

		for (int index = (start + 1); index < SIZE; index++)
		{
			if (*(QtyPtr[index]) > *maxValue)
			{
				maxValue = QtyPtr[index];
				tempId = idPtr[index];
				maxElement = index;
			}
		}
		QtyPtr[maxElement] = QtyPtr[start];
		idPtr[maxElement] = idPtr[start];
		QtyPtr[start] = maxValue;
		idPtr[start] = tempId;
	}

	//null character
	char end[] = { '\0' };
	//Today's date
	char date[DATEADDEDSIZE];
	//prompts the user to enter the date
	cout << "Please enter today's date: ";
	cin.ignore();
	cin.getline(date, DATEADDEDSIZE);

	//Greeting which includes today's date
	cout << "\n\n\t\t Serendipity Booksellers Inventory Quantity on Hand Report\n";
	cout << "Date: \t\t" << date << endl;
	cout << "\t____________________________________________________";

	//Displays the quantity on Hand report
	for (int index = 0; index < SIZE; index++)
	{
		if (strcmp(book[*(idPtr[index])].getISBN(), end) != 0) //no empty strings
		{
			cout << "\n_________________________________________________________";
			cout << "\n\nTitle:\t\t\t" << book[*(idPtr[index])].getTitle();
			cout << "\nISBN:\t\t\t" << book[*(idPtr[index])].getISBN();
			cout << "\nQuantity on Hand:\t" << *QtyPtr[index];
		}
	}
	cout << endl;

	//display the ending of the report
	cout << "End of Inventory Quantity on Hand Report..." << endl;
	//Pause the report on the screen
	cout << "Please press any key to continue to the next screen: ";
	cin.ignore();
	cin.get();

}
//function for the listing by cost in the Reports function
void repCost()
{
	int id[SIZE]; //Original subscript tracking array
	int* idPtr[SIZE]; // pointers to tracking array
	double* wholePtr[SIZE]; //array of pointers to wholesale array
	double bookWhole[SIZE];  //array of wholesale object accessor values

	for (int index = 0; index < SIZE; index++)
	{
		id[index] = index;   //sets the subscript tracking array to each individual subscript
		idPtr[index] = &id[index];  //sets the pointers to tracking array to the
										  //address of the subscript tracking array
		bookWhole[index] = book[index].getWholesale();
		wholePtr[index] = &bookWhole[index]; //sets the wholesale pointers to the address
											// of the wholesale array
	}

	int maxElement;  //max subscript value
	int* tempId;     //points to a temporary value which will be used
	double* maxValue;   //points to the max value within the qty on hand array
	//selection sort algorithm which will arrange these items by their wholesale value
	// in descending order
	for (int start = 0; start < SIZE - 1; start++)
	{
		maxElement = start;
		maxValue = &bookWhole[start];
		tempId = idPtr[start];
		for (int index = (start + 1); index < SIZE; index++)
		{
			if (*(wholePtr[index]) > *maxValue)
			{
				maxValue = wholePtr[index];
				tempId = idPtr[index];
				maxElement = index;
			}
		}
		wholePtr[maxElement] = wholePtr[start];
		idPtr[maxElement] = idPtr[start];
		wholePtr[start] = maxValue;
		idPtr[start] = tempId;
	}

	//null character
	char end[] = { '\0' };
	//Today's date
	char date[DATEADDEDSIZE];
	//prompts the user to enter the date
	cout << "Please enter today's date: ";
	cin.ignore();
	cin.getline(date, DATEADDEDSIZE);

	//Greeting which includes today's date
	cout << "\n\n\t\t Serendipity Booksellers Inventory Wholesale Value Report\n";
	cout << "Date: \t\t" << date << endl;
	cout << "\t____________________________________________________";

	//Displays the quantity on Hand report
	for (int index = 0; index < SIZE; index++)
	{
		if (strcmp(book[*(idPtr[index])].getISBN(), end) != 0) //no empty strings
		{
			cout << "\n_________________________________________________________";
			cout << "\n\nTitle:\t\t\t" << book[*(idPtr[index])].getTitle();
			cout << "\nISBN:\t\t\t" << book[*(idPtr[index])].getISBN();
			cout << "\nWholesale Value:\t$" << *wholePtr[index];
		}
	}
	cout << endl;

	//display the ending of the report
	cout << "End of Inventory Wholesale Value Report..." << endl;
	//Pause the report on the screen
	cout << "Please press any key to continue to the next screen: ";
	cin.ignore();
	cin.get();
}
//function for the listing by age in the Reports function
void repAge()
{
	int id[SIZE]; //Original subscript tracking array
	int* idPtr[SIZE]; // pointers to tracking array
	char* datePtr[SIZE]; //array of pointers to date added array

	for (int index = 0; index < SIZE; index++)
	{
		id[index] = index;   //sets the subscript tracking array to each individual subscript
		idPtr[index] = &id[index];  //sets the pointers to tracking array to the
										  //address of the subscript tracking array
		datePtr[index] = book[index].getDateAdded(); //sets the date added pointers to the address
											// of the date added array
	}

	int maxElement;  //max subscript value
	int* tempId;     //points to a temporary value which will be used
	char* maxValue;   //points to the max value within the date added array
	//selection sort algorithm which will arrange these items by their date added
	// in descending order
	for (int start = 0; start < SIZE - 1; start++)
	{
		maxElement = start;
		maxValue = datePtr[start];
		tempId = idPtr[start];
		for (int h = (start + 1); h < SIZE; h++)
		{
			if (*(datePtr[h]) > *maxValue)
			{
				maxValue = datePtr[h];
				tempId = idPtr[h];
				maxElement = h;
			}
		}
		datePtr[maxElement] = datePtr[start];
		idPtr[maxElement] = idPtr[start];
		datePtr[start] = maxValue;
		idPtr[start] = tempId;
	}

	//Null character
	char end[] = { '\0' };
	//Today's date
	char date[DATEADDEDSIZE];
	//prompts the user to enter the date
	cout << "Please enter today's date: ";
	cin.ignore();
	cin.getline(date, DATEADDEDSIZE);

	//Greeting which includes today's date
	cout << "\n\n\t\t Serendipity Booksellers Inventory by Date Added Report\n";
	cout << "Date: \t\t" << date << endl;
	cout << "\t____________________________________________________";

	//Displays the date added report
	for (int j = 0; j < SIZE; j++)
	{
		if (strcmp(book[j].getISBN(), end) != 0) //no empty strings
		{
			cout << "\n_________________________________________________________";
			cout << "\n\nTitle:\t\t\t" << book[*(idPtr[j])].getTitle();
			cout << "\nISBN:\t\t\t" << book[*(idPtr[j])].getISBN();
			cout << "\nDate Added:\t" << book[*(idPtr[j])].getDateAdded();
		}
	}
	cout << endl;

	//display the ending of the report
	cout << "End of Inventory by Date Added Report..." << endl;
	//Pause the report on the screen
	cout << "Please press any key to continue to the next screen: ";
	cin.ignore();
	cin.get();
}
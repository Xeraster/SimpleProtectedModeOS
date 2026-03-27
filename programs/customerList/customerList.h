//TODO: USE AS A BASE FOR TEMPLATED LINKED LIST IN C++ OS
//#include <iostream>
//#include <ctime>
//using namespace std;	//LOL! don't have that here

#ifndef CUSTOMERLIST_H
#define CUSTOMERLIST_H

#include "linkedList.h"

int customerListInit()
{
	string customerName;
	string service;
	linkedList customerList;
	bool exit = false;
	do {
		consoleNewLine();
		printString("1. New customer arrival", 0x0F);
		consoleNewLine();
		printString("2. Serve customer", 0x0F);
		consoleNewLine();
		printString("3. Customer left - remove from waiting list", 0x0F);
		consoleNewLine();
		printString("4. List all customer waiting for service", 0x0F);
		consoleNewLine();
		printString("5. Quit", 0x0F);
		consoleNewLine();
		printString("Please enter the number of your choice: ", 0x0F);

		int option;
		//cin >> option;
		option = waitChar();
		printChar(option, 0x0E);
		option-=48;//int not char

		switch (option)
		{
			case 1:
				consoleNewLine();
				printString("Please enter the customer's name: ", 0x0F);
				//string customerName;
				//string service;
				//cin >> customerName;
				customerName = waitString(true);
				consoleNewLine();
				printString("Please enter the requested service: ", 0x0F);
				service = waitString(true);
				customerList.insertNode(customerName, service);

				break;
			case 2:
				//serve the customer at the highest part of the queue
				customerList.serveCustomer();
				break;
			case 3:
				//cout << "not implemented" << endl;
				//cin >> customerName;
				waitChar();
				if (customerList.deleteNode(customerName))
				{
					printString(customerName, 0x0F);
					printString(" left and was not served ", 0x0F);
					consoleNewLine();
				}
				else
				{
					printString(customerName, 0x0F);
					printString(" not found. They were probably already served and didn't get marked for it", 0x0F);
					consoleNewLine();
				}
				break;
			case 4:
				customerList.listAll();
				break;
			case 5:
				exit = true;
				break;
			default:
				printString("Error: incorrect option selected", 0x0F);
				consoleNewLine();
				break;
		}
	} while (!exit);
	return 0;
}

#endif
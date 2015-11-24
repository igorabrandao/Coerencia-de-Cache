/*! \file */ 
/*! \brief Input.cpp.
 *
 *  Implements the functions from Input class.
*/

#include "Input.h"

/********************************************//**
* Get the input
***********************************************/
int 
Input::GetInput()
{
	int choice;
	cin >> choice;
	return choice;
}

/********************************************//**
* Display the menu
***********************************************/
void
Input::DisplayMainMenu()
{
	cout << endl << "************************************************" << endl;
	cout << "* Cache Coherence Model\n";
	cout << "************************************************" << endl << endl;
	cout << "Please make your selection\n";
	cout << "1 - Start game\n";
	cout << "2 - Options\n";
	cout << "3 - Quit\n";
	cout << endl << "************************************************" << endl << endl;
	cout << "Choose an option: ";
}

/********************************************//**
* Start the application menu
***********************************************/
void
Input::Start()
{
	int choice = 0;

	do
	{
		system("clear");
		DisplayMainMenu();
		choice = GetInput();

		switch( choice )
		{
			case 1:
				cout << "difficulty stuff";
			break;
			case 2:
				cout << "sound stuff";
			break;
			case 3:
			break;
			default:
			break;
		}

	} while ( choice != 3 );
}
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <iostream>

#include "cache.h"
#include "directory.h"

using namespace std;

ifstream fin;
FILE * pFile;

int cache_size;			/*! L1 = 8k - 64k / L2 = 128k - 2mb */
int cache_assoc;
int blk_size;			/*! 4b - 64b */
int num_processors;		/*! 1, 2, 4, 8 */
//int protocol;			/*! 0:MSI, 1:MESI, 2:MOESI */
string fname;

void return_to_menu();

/**
 * Get the user choice
*/
int getInput()
{
	int choice;
	cin >> choice;
	return choice;
}

/**
 * Print the cache basic settings
*/
void printBasicSettings()
{
	/*! Print the basic settings */
	printf("========= CACHE BASIC SETTINGS =========\n");
	printf("%-23s%d\n", "L1_SIZE:", cache_size);
	printf("%-23s%d\n", "L1_ASSOC:", cache_assoc);
	printf("%-23s%d\n", "L1_BLOCKSIZE:", blk_size);
	printf("%-23s%d\n", "NUMBER OF PROCESSORS:", num_processors);
	//printf("%-23s%s\n", "COHERENCE PROTOCOL:", "MESI");
	printf("%-23s%s\n", "TRACE FILE:", fname.c_str());
	printf("========================================\n");
}

/**
 * Print the cache actual status
*/
void printCache()
{
	/*! Print the basic settings */
	//printf("========= Simulation results (Cache_%i) =========\n", i);
	//cachesArray[i]->printStats();
	printf("==================================================\n");
}

/**
 * Run the test with the trace file
 *
 * @showSettings_ => bool to set if the cache settings will be
 * 					 displayed or not
*/
void runAutomaticTest( bool showSettings_ )
{
	if ( showSettings_ == true )
		printBasicSettings();

	cout << endl << "<<< Inform the automatic test file: ";
	cin >> fname;
	cout << endl;

 	/*! Try to read the trace file */
	pFile = fopen (fname.c_str(),"r");

	if( pFile == 0 )
	{
		cout << endl << "<<< Problem reading the automatic test file >>>" << endl;
		runAutomaticTest(false);
	}
	else
	{
		cout << endl << "<<< Reading file... >>>" << endl << endl;
	}

	/*! Generate an array of caches accordling to the processors number */
 	Cache *cachesArray[num_processors];

 	for ( int i = 0; i < num_processors; i++ )
 	{
 		cachesArray[i] = new Cache(cache_size, cache_assoc, blk_size, i);
 	}

 	/*! Create the cache directory accordling to the basic settings */
 	Directory *directory = new Directory(cache_size, blk_size, num_processors);

	///******************************************************************//
	//**read trace file,line by line,each(processor#,operation,address)**//
	//*****propagate each request down through memory hierarchy**********//
	//*****by calling cachesArray[processor#]->Access(...)***************//
	///******************************************************************//
	int processor_number;
	uchar op;
	ulong address;

	while ( fscanf(pFile, "%i %c %lx", &processor_number, &op, &address) == 3 )
	{
		cachesArray[processor_number]->Access(address, op, cachesArray, directory);
	}

	/*! Close the file reading */
	fclose(pFile);

	/*! Print all caches statistics */
	for (int i = 0; i < num_processors; i++)
	{
		printf("========= Simulation results (Cache_%i) =========\n", i);
		cachesArray[i]->printStats();
		printf("==================================================\n");
	}

	return_to_menu();
}

/**
 * Run the test manually
*/
void runManualTest()
{
	/*! Variables to receive the commands parameter */
	int processor_number;	//!< processor number
	uchar op;				//!< operation type W = write / R = read
	ulong address;			//!< cache address

	/*! Auxilary variable */
	int process_status = 0;

	/*! Print the basic settings */
	cout << endl;
	printBasicSettings();

	/*! Generate an array of caches accordling to the processors number */
 	Cache *cachesArray[num_processors];

 	for ( int i = 0; i < num_processors; i++ )
 	{
 		cachesArray[i] = new Cache(cache_size, cache_assoc, blk_size, i);
 	}

 	/*! Create the cache directory accordling to the basic settings */
 	Directory *directory = new Directory(cache_size, blk_size, num_processors);

	/*! Print the basic settings */
	printf("\n======= COMMANDS TO MANUAL INPUT =======\n");
	printf("%-23s\n", "Operation: <processor number>, <operation: r/w>, <cache address>");
	printf("%-23s\n", "Quit: <q>");
	printf("========================================\n\n");	

	do
	{
		/*! Check if all arguments was passed */
		cout << "<<< Inform the command: ";
		if ( scanf("%i %c %lx", &processor_number, &op, &address) == 3 )
		{
		    cachesArray[processor_number]->Access(address, op, cachesArray, directory);
		    process_status = 1;
		}
	}
	while ( getchar() != 'q' );

	/*! Print all caches statistics */
	if ( process_status == 1 )
	{
		for (int i = 0; i < num_processors; i++)
		{
			printf("========= Simulation results (Cache_%i) =========\n", i);
			cachesArray[i]->printStats();
			printf("==================================================\n");
		}
	}

	return_to_menu();
}

/**
 * Get the basic settings to simulate the cache
*/
void initializeCache()
{
	/*! Initialize the basic settings */
	cout << endl << "<<< Inform the cache size in bytes: ";
	cin >> cache_size;
	cout << endl;

	/*cout << "<<< Inform the cache association set: ";
	cin >> cache_assoc;
	cout << endl;*/
	cache_assoc = 8;

	cout << "<<< Inform the block size in bytes: ";
	cin >> blk_size;
	cout << endl;

	cout << "<<< Inform the processors number: ";
	cin >> num_processors;
	cout << endl;

	/*cout << "<<< Inform the protocol [0 = MSI / 1 = MESI / 2 = MOESI]: ";
	cin >> protocol;
	cout << endl;*/

	printBasicSettings();
	return_to_menu();
}

/**
 * Handle the user input choice
*/
void inputHandler( int choice_ )
{
	switch( choice_ )
	{
		/*! Initialize the cache */
		case 1:
			initializeCache();
			break;

		/*! Automatic test */
		case 2:
			runAutomaticTest(true);
			break;

		/*! Manual test */
		case 3:
			runManualTest();
			break;

		/*! Print all caches */
		case 4:
			break;

		/*! Print cache <cache number> */
		case 5:
			printCache();
			break;

		/*! Exit */
		case 6:
			exit(0);
			break;

        default:
			exit(0);
			break;
	}
}

/**
 * Display the menu
*/
void displayMainMenu()
{
	system("clear");

	cout << endl << "**********************************************************" << endl;
	cout << "* Cache Coherence Based on Directory Protocol Simulation\n";
	cout << "**********************************************************" << endl << endl;
	cout << "Please make your selection\n";
	cout << "1 - Initialize the cache\n";
	cout << "2 - Automatic test\n";
	cout << "3 - Manual test\n";
	cout << "4 - Print all caches\n";
	cout << "5 - Print cache <cache number>\n";
	cout << "6 - Quit\n";
	cout << endl << "**********************************************************" << endl << endl;
	cout << "Choose an option: ";

	inputHandler(getInput());
}

/**
 * Function to return to menu
*/
void return_to_menu()
{
    char opt;
    cout << endl << "Would you like to return? (y or n): ";
    cin >> opt;

    if ( opt == 'y' || opt == 'Y' )
        displayMainMenu();
    else
        exit(0);
}

/**
 * Main function
*/
int main( int argc, char *argv[] )
{
	displayMainMenu();
	cout << "<<< Finish with success! >>>" << endl << endl;
	exit(0);
}
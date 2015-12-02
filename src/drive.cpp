#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <string.h>
using namespace std;

#include "cache.h"
#include "directory.h"

int main(int argc, char *argv[])
{
	
	ifstream fin;
	FILE * pFile;

	if(argv[1] == NULL){
		 printf("input format: ");
		 //printf("./smp_cache <cache_size> <assoc> <block_size> <num_processors> <protocol> <trace_file> \n");
		 printf("./smp_cache <cache_size> <assoc> <block_size> <num_processors> <trace_file> \n");
		 exit(0);
        }

	/*****uncomment the next five lines*****/
	int cache_size = atoi(argv[1]);
	int cache_assoc= atoi(argv[2]);
	int blk_size   = atoi(argv[3]);
	int num_processors = atoi(argv[4]);/*1, 2, 4, 8*/
	//int protocol   = atoi(argv[5]);	 /*0:MSI, 1:MESI, 2:MOESI*/
	char *fname =  (char *)malloc(20);
 	fname = argv[5];

	
	//****************************************************//
	printf("===== 456 DSM (MESI with Full-bit Vector Simulator Configuration =====\n");
	//*******print out simulator configuration here*******//
	printf("%-23s%d\n", "L1_SIZE:", cache_size);
	printf("%-23s%d\n", "L1_ASSOC:", cache_assoc);
	printf("%-23s%d\n", "L1_BLOCKSIZE:", blk_size);
	printf("%-23s%d\n", "NUMBER OF PROCESSORS:", num_processors);
	//printf("%-23s%s\n", "COHERENCE PROTOCOL:", "MESI");
	printf("%-23s%s\n", "TRACE FILE:", fname);
	//****************************************************//

 
	//*********************************************//
  //*****create an array of caches here**********//
 	Cache *cachesArray[num_processors];
 	for (int i = 0; i < num_processors; i++) {
 		cachesArray[i] = new Cache(cache_size, cache_assoc, blk_size, i);
 	}

 	//*****Create the directory*****//
 	Directory *directory = new Directory(cache_size, blk_size, num_processors);

	//*********************************************//	

	pFile = fopen (fname,"r");
	if(pFile == 0)
	{   
		printf("Trace file problem\n");
		exit(0);
	}
	///******************************************************************//
	//**read trace file,line by line,each(processor#,operation,address)**//
	//*****propagate each request down through memory hierarchy**********//
	//*****by calling cachesArray[processor#]->Access(...)***************//
	///******************************************************************//

	int processor_number;
	uchar op;
	ulong address;

	while (fscanf(pFile, "%i %c %lx", &processor_number, &op, &address) == 3) {
		cachesArray[processor_number]->Access(address, op, cachesArray, directory);
	}


	fclose(pFile);

	//********************************//
	//print out all caches' statistics //
	//********************************//
	
	for (int i = 0; i < num_processors; i++) {
		printf("===== Simulation results (Cache_%i)      =====\n", i);
		cachesArray[i]->printStats();
	}
}

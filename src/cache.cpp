#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cache.h"
#include "directory.h"
using namespace std;

Cache::Cache(int s,int a,int b, int n )
{
   ulong i, j;
   reads = readMisses = writes = 0; 
   writeMisses = writeBacks = currentCycle = 0;
   
   processor_number = (ulong)(n);

   size       = (ulong)(s);
   lineSize   = (ulong)(b);
   assoc      = (ulong)(a);   
   sets       = (ulong)((s/b)/a);
   numLines   = (ulong)(s/b);
   log2Sets   = (ulong)(log2(sets));
   log2Blk    = (ulong)(log2(b));
  
   //*******************//
   //initialize your counters here//
   //*******************//

   invalidations = cacheToCacheTransfers = 0;
 
   tagMask =0;
   for(i=0;i<log2Sets;i++)
   {
		tagMask <<= 1;
        tagMask |= 1;
   }
   
   /**create a two dimentional cache, sized as cache[sets][assoc]**/ 
   cache = new cacheLine*[sets];
   for(i=0; i<sets; i++)
   {
      cache[i] = new cacheLine[assoc];
      for(j=0; j<assoc; j++) 
      {
	   cache[i][j].invalidate();
      }
   }
}

/**you might add other parameters to Access()
since this function is an entry point 
to the memory hierarchy (i.e. caches)**/
void Cache::Access(ulong addr, uchar op, Cache **cachesArray, Directory *directory)
{
	currentCycle++;/*per cache global counter to maintain LRU order 
			among cache ways, updated on every cache access*/
        	
   if(op == 'w') writes++;
	else          reads++;
	
	cacheLine * line = findLine(addr);
   ulong state;


	if(line == NULL)/*miss*/
	{
      if(op == 'w') {

         writeMisses++;
         directory->ReadX(addr, processor_number, cachesArray);
         state = MODIFIED;
      }
		else {
         readMisses++;
         state = directory->Read(addr, processor_number, cachesArray); //Returns whether or not the line is shared

      }

		cacheLine *newline = fillLine(addr, directory);
   	newline->setFlags(state);

		
	}
	else //Hit
	{
      /**since it's a hit, update LRU and update dirty flag**/

		updateLRU(line);
      assert(line->getFlags() != INVALID);
		if(op == 'w') {
         state = line->getFlags();
         if (state == SHARED) {

            directory->Upgrade(addr, processor_number, cachesArray);
         }
         line->setFlags(MODIFIED);
      }
	}
}

//Find address and invalidate cacheLine containing it
void Cache::Invalidate(ulong addr) {
   cacheLine *line = findLine(addr);
   assert(line != NULL);
   if(line != NULL) {
      line->invalidate();
      invalidations++;
   }
}

//Find address and intervent cacheLine containing it
void Cache::Intervent(ulong addr) {
   cacheLine *line = findLine(addr);
   assert(line != NULL);
   if(line != NULL)
      line->setFlags(SHARED);
}

/*look up line*/
cacheLine * Cache::findLine(ulong addr)
{
   ulong i, j, tag, pos;
   
   pos = assoc;
   tag = calcTag(addr);
   i   = calcIndex(addr);
  
   for(j=0; j<assoc; j++)
	if(cache[i][j].isValid())
	        if(cache[i][j].getTag() == tag)
		{
		     pos = j; break; 
		}
   if(pos == assoc)
	return NULL;
   else
	return &(cache[i][pos]); 
}

/*upgrade LRU line to be MRU line*/
void Cache::updateLRU(cacheLine *line)
{
  line->setSeq(currentCycle);  
}

/*return an invalid line as LRU, if any, otherwise return LRU line*/
cacheLine * Cache::getLRU(ulong addr)
{
   ulong i, j, victim, min;

   victim = assoc;
   min    = currentCycle;
   i      = calcIndex(addr);
   
   for(j=0;j<assoc;j++)
   {
      if(cache[i][j].isValid() == 0) return &(cache[i][j]);     
   }   
   for(j=0;j<assoc;j++)
   {
	 if(cache[i][j].getSeq() <= min) { victim = j; min = cache[i][j].getSeq();}
   } 
   assert(victim != assoc);
   
   return &(cache[i][victim]);
}

/*find a victim, move it to MRU position*/
cacheLine *Cache::findLineToReplace(ulong addr)
{
   cacheLine * victim = getLRU(addr);
   updateLRU(victim);
  
   return (victim);
}

/*allocate a new line*/
cacheLine *Cache::fillLine(ulong addr, Directory* directory)
{ 

   ulong tag;
  
   cacheLine *victim = findLineToReplace(addr);
   assert(victim != 0);

   // If victim is MODIFIED it needs to be written back
   if(victim->getFlags() == MODIFIED)
      writeBack(addr);
   
   // If victim is not INVALID, notify the directory that it is being evicted
   if(victim->getFlags() != INVALID)
      directory->UnCache(calcAddr4Tag(victim->getTag()), processor_number);
    	
   tag = calcTag(addr);   
   victim->setTag(tag);
   /**note that this cache line has been already 
      upgraded to MRU in the previous function (findLineToReplace)**/

   return victim;
}

void Cache::printStats()
{ 
	//printf("===== Simulation results      =====\n");
	/****print out the rest of statistics here.****/
	/****follow the ouput file format**************/
   double missRate = (double)(readMisses + writeMisses) / (reads + writes);

   printf("%-50s%lu\n", "01. number of reads:", reads);
   printf("%-50s%lu\n", "02. number of read misses:", readMisses);
   printf("%-50s%lu\n", "03. number of writes:", writes);
   printf("%-50s%lu\n", "04. number of write misses:", writeMisses);
   printf("%-50s%lf\n", "05. total miss rate:", missRate);
   printf("%-50s%lu\n", "06. number of writebacks:", writeBacks);
   printf("%-50s%lu\n", "07. number of invalidations:", invalidations);
   printf("%-50s%lu\n", "08. number of cache to cache transfers:", cacheToCacheTransfers);
}

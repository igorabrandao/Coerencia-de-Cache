#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "directory.h"
#include "cache.h"
using namespace std;

Directory::Directory(int cache_size, int blk_size, int num_processors)
{
  numBlocks = (ulong) cache_size * num_processors / blk_size;
  numProcs = (ulong)num_processors;
  log2Blk = (ulong)(log2(blk_size));

  directory = new directoryBlock*[numBlocks];
  for (ulong i = 0; i < numBlocks; i++) {
    directory[i] = new directoryBlock(num_processors);
  }
}

ulong Directory::Read(ulong addr, ulong processor_number, Cache **cachesArray)
{

   directoryBlock *block = findBlock(addr);
   ulong cacheState;
   if (block == NULL) { //Block not found

      directoryBlock *newBlock = fillBlock(addr);
      assert(newBlock != NULL);
      newBlock->setVector(processor_number, 1);
      newBlock->setFlags(EXCLUSIVE_MODIFIED_D);
      cacheState = EXCLUSIVE;
      block = newBlock;

   } else { //Block found
      ulong state = block->getFlags();

      if (state == EXCLUSIVE_MODIFIED_D) {

         //uchar *vector = block->getVector();
         ulong numSharers = 0;
         for (ulong i = 0; i < numProcs; i++) {
            if (block->getVector(i) == 1) {
               numSharers++;
               cachesArray[i]->Intervent(addr);
               //break;
            }
         }
         assert(numSharers == 1);
         block->setVector(processor_number, 1);
         block->setFlags(SHARED_D);
         cacheState = SHARED;
         cachesArray[processor_number]->CacheToCacheTransfer();

      } else {
         block->setVector(processor_number, 1);
         cacheState = SHARED;
         //cachesArray[processor_number]->CacheToCacheTransfer();
      }
   }



   return cacheState;
}

void Directory::ReadX(ulong addr, ulong processor_number, Cache **cachesArray)
{

   directoryBlock *block = findBlock(addr);
   if (block == NULL) { //Block not found

      directoryBlock *newBlock = fillBlock(addr);
      newBlock->setVector(processor_number, 1);
      newBlock->setFlags(EXCLUSIVE_MODIFIED_D);

   } else { //Block found
      ulong state = block->getFlags();
      if (state != UNCACHED_D) { //Send out invalidations
         assert(state != UNCACHED_D);
         if (state == EXCLUSIVE_MODIFIED_D)
            cachesArray[processor_number]->CacheToCacheTransfer();
         for (ulong i = 0; i < numProcs; i++) {
            if ((block->getVector(i) == 1) && (i != processor_number)) {
               cachesArray[i]->Invalidate(addr);
            }
         }
      }
      block->clearVector();
      block->setVector(processor_number, 1);
      block->setFlags(EXCLUSIVE_MODIFIED_D);
   }
}

void Directory::Upgrade(ulong addr, ulong processor_number, Cache **cachesArray)
{

   directoryBlock *block = findBlock(addr);
   assert(block != NULL);
   if (block != NULL) { //Block found, this should always be the case
      for (ulong i = 0; i < numProcs; i++) {
         if ((block->getVector(i) == 1) && (i != processor_number)) {

            cachesArray[i]->Invalidate(addr);
         }
      }
      block->clearVector();
      block->setVector(processor_number, 1);
      block->setFlags(EXCLUSIVE_MODIFIED_D);
   }


}

void Directory::Reply()
{

}

void Directory::ReplyD()
{

}

void Directory::UnCache(ulong addr, ulong processor_number)
{

   directoryBlock *block = findBlock(addr);
   assert(block != NULL);
   if (block != NULL) { //Block found
      block->setVector(processor_number, 0);
      ulong numSharers = 0;
      for (ulong i = 0; i < numProcs; i++) {
         if (block->getVector(i) == 1) {
            numSharers++;
         }
      }

      if (numSharers == 0) {
         block->invalidate();
      }
      //else if (numSharers == 1) //Don't know if this should be done, but seems reasonable
        // block->setFlags(EXCLUSIVE_MODIFIED_D);
   }
}

/*look up line*/
directoryBlock* Directory::findBlock(ulong addr)
{
   ulong i, tag;   
   tag = calcTag(addr);

  
   for (i = 0; i < numBlocks; i++) {
      if (directory[i]->isUsed()) {
         if (directory[i]->getTag() == tag)
            return directory[i];
      }
   }
   return NULL; 
}

/*find a victim, move it to MRU position*/
directoryBlock* Directory::findBlockToReplace(ulong addr)
{
   for (ulong i = 0; i < numBlocks; i++) {
      if (directory[i]->isUsed() == 0)
         return directory[i];
   }
   return NULL;
}

/*allocate a new line*/
directoryBlock* Directory::fillBlock(ulong addr)
{ 
   ulong tag;
   directoryBlock *victim = findBlockToReplace(addr);
   assert(victim != NULL);
   assert(victim->getTag() == 0);
   assert(victim->getFlags() == UNCACHED_D);
   for (ulong i = 0; i < numProcs; i++) {
      assert(victim->getVector(i) == 0);
   }

   tag = calcTag(addr);
   victim->setTag(tag);

   return victim;
}
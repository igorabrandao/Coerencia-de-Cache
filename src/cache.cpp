#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cache.h"
#include "directory.h"

using namespace std;

/**
 * Class constructor
 *
 * @s => cache size
 * @a => number of associations to the sets
 * @b => block size
 * @n => number of processors
*/
Cache::Cache( int s, int a, int b, int n )
{
    /*! Initialize the attributes */
    ulong i, j;
    reads = readMisses = writes = 0; 
    writeMisses = writeBacks = currentCycle = 0;

    processor_number = (ulong)(n);      //!< number of processors

    size       = (ulong)(s);            //!< cache size
    lineSize   = (ulong)(b);            //!< block/line size
    assoc      = (ulong)(a);            //!< number of associations to the sets
    sets       = (ulong)((s/b)/a);      //!< groups of blocks
    numLines   = (ulong)(s/b);          //!< number of blocks/lines
    log2Sets   = (ulong)(log2(sets));   //!< binary base of sets (group of blocks)
    log2Blk    = (ulong)(log2(b));      //!< binary base of lineSize (block/line size)

    /*! Initialize the coherence cache counters here */
    invalidations   = cacheToCacheTransfers = 0;
    tagMask         = 0;

    for ( i = 0; i < log2Sets; i++ )
    {
        tagMask <<= 1;
        tagMask |= 1;
    }

    /**
     * Create a two dimentional cache, sized as cache[sets][assoc]
     * Set Associative Scheme
    */
    cache = new cacheLine*[sets];

    for ( i = 0; i < sets; i++ )
    {
        cache[i] = new cacheLine[assoc];

        for ( j = 0; j < assoc; j++ ) 
        {
            cache[i][j].invalidate();
        }
    }
}

/**
 * Process the command itself
 * You might add other parameters to Access()
 * since this function is an entry point 
 * to the memory hierarchy (i.e. caches)
 *
 * @addr => cache address
 * @op => command
 * @cachesArray => cache Array pointer
 * @directory => directory pointer
*/
void Cache::Access( ulong addr, uchar op, Cache **cachesArray, Directory *directory )
{
    /*! per cache global counter to maintain LRU order 
    among cache ways, updated on every cache access*/
    currentCycle++;

    /*! Check the operation */
    if ( op == 'w' ) writes++;
    else reads++;

    /*! Pointer to access the cache line */
	cacheLine * line = findLine(addr);
    ulong state;

    /*! Cache MISS */
	if ( line == NULL )
	{
        if ( op == 'w' )
        {
            writeMisses++;
            directory->ReadX(addr, processor_number, cachesArray);
            state = MODIFIED;
        }
        else
        {
            readMisses++;

            /*! Returns whether or not the line is shared */
            state = directory->Read(addr, processor_number, cachesArray);
        }

        cacheLine *newline = fillLine(addr, directory);
        newline->setFlags(state);
	}
    /*! Cache HIT */
	else
	{
        /**since it's a hit, update LRU and update dirty flag**/
        updateLRU(line);
        assert(line->getFlags() != INVALID);

        if ( op == 'w' )
        {
            state = line->getFlags();

            if (state == SHARED)
            {
                directory->Upgrade(addr, processor_number, cachesArray);
            }
            line->setFlags(MODIFIED);
        }
	}
}

/**
 * Find address and invalidate cacheLine containing it
 *
 * @addr => cache address
*/
void Cache::Invalidate( ulong addr )
{
    cacheLine *line = findLine(addr);
    assert(line != NULL);

    if ( line != NULL )
    {
        line->invalidate();
        invalidations++;
    }
}

/**
 * Find address and intervent cacheLine containing it
 *
 * @addr => cache address
*/
void Cache::Intervent(ulong addr)
{
    cacheLine *line = findLine(addr);
    assert(line != NULL);

    if ( line != NULL )
        line->setFlags(SHARED);
}

/**
 * Look up line
 *
 * @addr => cache address
*/
cacheLine * Cache::findLine(ulong addr)
{
    ulong i, j, tag, pos;

    pos = assoc;
    tag = calcTag(addr);
    i   = calcIndex(addr);

    for ( j = 0; j < assoc; j++ )
        if ( cache[i][j].isValid() )

    if ( cache[i][j].getTag() == tag )
    {
        pos = j; break; 
    }

    if ( pos == assoc )
        return NULL;
    else
        return &(cache[i][pos]);
}

/**
 * Upgrade LRU line to be MRU line
 *
 * @line => pointer to the cache line
*/
void Cache::updateLRU( cacheLine *line )
{
    line->setSeq(currentCycle);
}

/**
 * Return an invalid line as LRU, if any, otherwise return LRU line
 *
 * @addr => cache address
*/
cacheLine * Cache::getLRU(ulong addr)
{
    ulong i, j, victim, min;

    victim = assoc;
    min    = currentCycle;
    i      = calcIndex(addr);

    for ( j = 0; j < assoc; j++ )
    {
        if ( cache[i][j].isValid() == 0 )
            return &(cache[i][j]);     
    }

    for ( j = 0; j < assoc; j++ )
    {
        if ( cache[i][j].getSeq() <= min )
        {
            victim = j;
            min = cache[i][j].getSeq();
        }
    }

    assert(victim != assoc);
    return &(cache[i][victim]);
}

/**
 * Find a victim, move it to MRU position
 *
 * @addr => cache address
*/
cacheLine *Cache::findLineToReplace( ulong addr )
{
    cacheLine * victim = getLRU(addr);
    updateLRU(victim);

    return (victim);
}

/**
 * Allocate a new line
 *
 * @addr => cache address
 * @directory => directory pointer
*/
cacheLine *Cache::fillLine( ulong addr, Directory* directory )
{
    /*! Auxiliary variables */
    ulong tag;

    cacheLine *victim = findLineToReplace(addr);
    assert(victim != 0);

    /*! If victim is MODIFIED it needs to be written back */
    if(victim->getFlags() == MODIFIED)
    writeBack(addr);

    /*! If victim is not INVALID, notify the directory that it is being evicted */
    if ( victim->getFlags() != INVALID )
        directory->UnCache(calcAddr4Tag(victim->getTag()), processor_number);

    tag = calcTag(addr);   
    victim->setTag(tag);

    /**
     *note that this cache line has been already 
     *upgraded to MRU in the previous function (findLineToReplace)
    */

    return victim;
}

/**
 * Print the statistics related to the cache reads, writes,
 * misses and others.
*/
void Cache::printStats()
{
    /*! Print the caches statistics */
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

/**
 * Print the cache actual state
 *
 * @addr => cache address
*/
void Cache::printCache( ulong addr )
{
    ulong i, j;

    i   = calcIndex(addr);

    for ( j = 0; j < assoc; j++ )
    {
        cout << "[" << i << "][" << j << "] = " << cache[i][j].getSeq() << endl;
    }
}
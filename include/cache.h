/*!
    <PRE>
    SOURCE FILE : cache.h
    DESCRIPTION.: cache class - with implementation.
    AUTHORS.....: Igor A. Brandão
    CONTRIBUTORS: Igor A. Brandão
    LOCATION....: IMD/UFRN.
    SATARTED ON.: NOV/2015
    CHANGES.....: Functions implemented.

    TO COMPILE..: Use makefile.
    OBS.........: Part of the AOC Project.

    LAST UPDATE.: November 30th, 2015.
    LAST UPDATE.: December 01st, 2015.
    </PRE>
*/

#ifndef CACHE_H
#define CACHE_H

#include <cmath>
#include <iostream>

/*! Custom classes */
#include "cacheLine.h"
#include "directory.h"

using namespace std;

class Directory;

/*! Cache class */
class Cache
{
    /*!
      * Protected section
    */
    protected:

        /*! Cache attributes */
        ulong size, lineSize, assoc, sets, log2Sets, log2Blk, tagMask, numLines;
        ulong reads, readMisses, writes, writeMisses, writeBacks;
        ulong invalidations, cacheToCacheTransfers;
        ulong processor_number;

        /*! Coherence cache counters here */

        /*! Basic members */
        cacheLine **cache;

        /**
         * Calculates the tag identifier
         *
         * @addr => address in hexadecimal
        */
        ulong calcTag( ulong addr )
        {
            return ( addr >> (log2Blk) );
        }

        /**
         * Calculates the index identifier
         *
         * @addr => address in hexadecimal
        */
        ulong calcIndex( ulong addr )
        {
            return ( (addr >> log2Blk) & tagMask );
        }

        /**
         * Calculates the tag address
         *
         * @addr => address in hexadecimal
        */
        ulong calcAddr4Tag( ulong tag )
        {
            return ( tag << (log2Blk) );
        }

    /*!
      * Public section
    */
    public:

        /*! Basic members */
        ulong currentCycle;

        /*! Class constructor */
        Cache( int, int, int, int );

        /*! Class destructor */
        ~Cache() { delete cache; }

        cacheLine * findLineToReplace( ulong addr );
        cacheLine * fillLine( ulong addr, Directory* directory );
        cacheLine * findLine( ulong addr );
        cacheLine * getLRU( ulong );

        /**
         * Get the read misses attribute
        */
        ulong getRM()
        {
            return readMisses;
        } 

        /**
         * Get the write misses attribute
        */
        ulong getWM()
        {
            return writeMisses;
        }

        /**
         * Get the reads attribute
        */
        ulong getReads()
        {
            return reads;
        }

        /**
         * Get the writes attribute
        */
        ulong getWrites()
        {
            return writes;
        }

        /**
         * Get the write back attribute
        */
        ulong getWB()
        {
            return writeBacks;
        }

        /*! Functions to handle bus transactions */
        void writeBack( ulong ) { writeBacks++; }
        void Access( ulong, uchar, Cache**, Directory* );
        void printStats();
        void printCache( ulong );
        void printAllCaches();
        void updateLRU( cacheLine * );

        void Invalidate( ulong );
        void Intervent( ulong );
        void CacheToCacheTransfer() { cacheToCacheTransfers++; }

        //******///
        //add other functions to handle bus transactions///
        //******///
};

#endif

/* --------------------- [ End of the cache.h header ] ------------------- */
/* ========================================================================= */
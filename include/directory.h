/*!
    <PRE>
    SOURCE FILE : directory.h
    DESCRIPTION.: directory class - with implementation.
    AUTHORS.....: Igor A. Brandão
    CONTRIBUTORS: Igor A. Brandão
    LOCATION....: IMD/UFRN.
    SATARTED ON.: NOV/2015
    CHANGES.....: Functions implemented.

    TO COMPILE..: Use makefile.
    OBS.........: Part of the AOC Project.

    LAST UPDATE.: November 30th, 2015.
    LAST UPDATE.: December 01st, 2015.
    LAST UPDATE.: December 02nd, 2015.
    </PRE>
*/

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <cmath>
#include <iostream>

/*! Custom classes */
#include "directoryBlock.h"
#include "cache.h"

using namespace std;

class Cache;

/*! Directory class */
class Directory
{
    /*!
      * Protected section
    */
    protected:

        /*! Directory attributes */
        ulong size, lineSize, assoc, sets, log2Sets, log2Blk, tagMask, numLines;
        ulong numBlocks, numProcs;
        //ulong reads,readMisses,writes,writeMisses,writeBacks;
        //ulong invalidations, cacheToCacheTransfers;

        /*! Coherence cache counters here */

        /*! Basic members */
        directoryBlock **directory;

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
        //ulong currentCycle;  
        
        /*! Class constructor */
        Directory( int, int, int );

        /*! Class destructor */
        ~Directory() { delete directory; }

        directoryBlock * findBlockToReplace( ulong addr );
        directoryBlock * fillBlock( ulong addr );
        directoryBlock * findBlock( ulong addr );

        ulong Read( ulong, ulong, Cache** );
        void ReadX( ulong, ulong, Cache** );
        void Upgrade( ulong, ulong, Cache** );

        void Reply();
        void ReplyD();

        void UnCache(ulong, ulong);

        /*ulong getRM(){return readMisses;} ulong getWM(){return writeMisses;} 
        ulong getReads(){return reads;}ulong getWrites(){return writes;}
        ulong getWB(){return writeBacks;}

        void writeBack(ulong)   {writeBacks++;}
        void Access(ulong,uchar);
        void printStats();*/

        //******///
        //add other functions to handle bus transactions///
        //******///
};

#endif

/* --------------------- [ End of the directory.h header ] ------------------- */
/* ========================================================================= */
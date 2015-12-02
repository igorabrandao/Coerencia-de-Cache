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

#ifndef DIRECTORYBLOCK_H
#define DIRECTORYBLOCK_H

#include <cmath>
#include <iostream>

using namespace std;

/*! Typedef definitions */
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

/*!
  * Enum to define the directory state (protocol)
*/
enum directoryStates
{
    EXCLUSIVE_MODIFIED_D = 0,
    SHARED_D,
    UNCACHED_D
};

/*! Directory Block class */
class directoryBlock 
{
    /*!
      * Protected section
    */
    protected:

        /*! Directory block attributes */
        ulong tag;
        ulong Flags;        //!< 0:invalid, 1:valid, 2:dirty 
        uchar *vector;      //!< Full bit-vector
        int numProcs;

    /*!
      * Public section
    */
    public:

        /**
         * Class constructor
         *
         * @n => number of blocks
        */
        directoryBlock( int n )
        {
            /*! Initialize directory block */
            tag = 0;
            Flags = UNCACHED_D;
            numProcs = n;
            vector = new uchar[n];
            for ( int i = 0; i < n; i++ )
            {
                vector[i] = 0;
            }
        }

        /**
         * Get the tag attribute
        */
        ulong getTag()
        {
            return tag;
        }

        /**
         * Get the Flags attribute
        */
        ulong getFlags()
        {
            return Flags;
        }

        /**
         * Get a specific block
         *
         * @i => block index
        */
        uchar getVector( int i )
        {
            return vector[i];
        }

        /**
         * Get the whole block
        */
        /*uchar * getVector()
        {
            return vector;
        }*/

        /**
         * Clear the whole block
        */
        void clearVector()
        {
            for ( int i = 0; i < numProcs; i++ )
            {
                vector[i] = 0;
            }

            /*! Change the status */
            Flags = UNCACHED_D;
        }

        /*! Set methods */
        void setVector( int procNum, uchar value ) { vector[procNum] = value; }
        void setFlags( ulong flags ) { Flags = flags; }
        void setTag( ulong a ) { tag = a; }

        /*! Status method */
        void invalidate() { tag = 0; clearVector(); }
        bool isUsed() { return ((Flags) != UNCACHED_D); }
};

#endif

/* --------------------- [ End of the directoryBlock.h header ] ------------------- */
/* ========================================================================= */
/*!
    <PRE>
    SOURCE FILE : cacheLine.h
    DESCRIPTION.: cacheLine class - with implementation.
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

#ifndef CACHELINE_H
#define CACHELINE_H

using namespace std;

/*! Typedef definitions */
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

/*!
  * Enum to define the cache state
*/
enum cacheStates
{
    MODIFIED = 0,   //!< Modified
    EXCLUSIVE,      //!< Exclusive
    SHARED,         //!< Shared
    INVALID         //!< Invalid
};

/*! cacheLine class */
class cacheLine 
{
    /*!
      * Protected section
    */
    protected:

        /*! Cache line attributes */
        ulong tag;
        ulong Flags;   //!< 0:modified, 1:exclusive, 2:shared, 3:invalid
        ulong seq;

    /*!
      * Public section
    */
    public:

        /*! Basic members */

        /**
         * Cache line constructor
        */
        cacheLine()
        { 
            tag = 0;
            Flags = INVALID;
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
         * Get the seq attribute
        */
        ulong getSeq()
        {
            return seq;
        }

        /**
         * Set the tag attribute
         *
         * @Seq => value to set the seq attribute
        */
        void setSeq( ulong Seq )
        {
            seq = Seq;
        }

        /**
         * Set the tag attribute
         *
         * @flags => value to set the Flags attribute
        */
        void setFlags( ulong flags )
        {  
            Flags = flags;
        }

        /**
         * Set the tag attribute
         *
         * @a => value to set the tag attribute
        */
        void setTag( ulong a )
        {
            tag = a;
        }

        /**
         * Set the cache line as invalidated
        */
        void invalidate()
        {
            tag = 0;
            Flags = INVALID;
        }

        /**
         * Check if the cache line is valid
        */
        bool isValid()
        {
            return ((Flags) != INVALID);
        }
};

#endif

/* --------------------- [ End of the cacheLine.h header ] ------------------- */
/* ========================================================================= */
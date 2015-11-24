/*!
    <PRE>
    SOURCE FILE : Classe.h
    DESCRIPTION.: Cache Template class - with implementation.
    AUTHORS.....: Igor A. Brandão
    CONTRIBUTORS: Igor A. Brandão
    LOCATION....: IMD/UFRN.
    SATARTED ON.: NOV/2015
    CHANGES.....: Functions implemented.

    TO COMPILE..: Use makefile.
    OBS.........: Part of the AOC Project.

    LAST UPDATE.: November 24th, 2015.
    </PRE>
*/

#ifndef CACHE_H
#define CACHE_H
 
using namespace std;

// ******************PUBLIC OPERATIONS*********************
// void * Allocate( size_t )    --> Allocates a volume of memory defined by the argument in bytes
// void Free( void * )          --> Receive a pointer to the reserved area and free this area to be used later
// void Debug()                 --> Print the block list

// ***********************ERRORS****************************


template < class T >
class Cache
{
    /*!
     * Public section
    */
    public:

        Cache();
        Cache( T theCached );
 
        T& Get();
        const T& Get() const;
        operator T&();
        operator const T&() const;
 
        void Set( const T& theCached );
        Cache< T >& operator =( const T& theCached );
 
        bool IsValid() const;
        operator bool() const;
        void Invalidate();
 
    /*!
     * Protected section
    */
    protected:

        T cached;
        bool isValid;
};

#include "Cache.inl"
 
#endif // CACHE_H
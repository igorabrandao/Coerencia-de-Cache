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

#ifndef INPUT_H
#define INPUT_H

#include <cstdlib>
#include <iostream>

using namespace std;

// ******************PUBLIC OPERATIONS*********************

// ***********************ERRORS****************************

class Input
{
    /*!
     * Public section
    */
    public:

        int GetInput();
        void DisplayMainMenu();
        void Start();

};

#endif // INPUT_H
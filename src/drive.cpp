/*! \file */ 
/*! \brief main.cpp.
 *
 *  Contains the main function and starts the program.
*/
#include <iostream>
#include "Cache.h"
#include "Input.h"

using namespace std;

/********************************************//**
* Main
***********************************************/
int main( int argc, char const *argv[] )
{
    /*! Input instance */
    Input input;

    /*! Start the application */
    input.Start();

    cout << "<<< Finish with success! >>>" << endl;

    /*! Main return */
    return 0;
}
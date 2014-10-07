// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MathFormObj.h"
#include "ConvertOM.h"
#include "MathML.h"

void MTRead( char* param, char* inputFileName, MathObj* obj ) 
{
	std::string par( param );
	if( par == "omath" ) 
    {
		ConvertFromOM( inputFileName, obj );
	}
	if( par == "mathml" )
	{
		MathMLParser A;
		A.Pars( inputFileName );
		*obj = *A.GetData( );
	}
	
}

void MTWrite( char* param, char* outputFileName, MathObj* obj ) 
{
	std::string par( param );
	if( par == "omath" ) 
    {
		ConvertToOM( outputFileName, obj );
	}
}


int main( int argc, char* argv[] ) 
{
	/*if( argc != 5 ) {
		std::cout << "nevernoe kolichestvo parametrov!" << std::endl;
		system( "pause" );
		return 0;
	}
	
	FormulaObj obj( NT_MAIN );
	MTRead( argv[1], argv[2], &obj );
	MTWrite( argv[3], argv[4], &obj );*/
	MathMLParser A;
	A.Pars( "inputMML.txt" );
	ConvertToOM( "output.txt", A.GetData() );

	system( "pause" );
	return 0;
}


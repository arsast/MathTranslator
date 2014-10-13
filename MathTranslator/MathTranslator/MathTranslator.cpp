// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MathFormObj.h"
#include "ConvertOM.h"
#include "MathML.h"

void MTConvert( char* inputParam, char* inputFileName, char* outputParam, char* outputFileName )
{
	FormulaObj obj( NT_MAIN );
	MathMLParser mmlparser;

	std::string inputPar( inputParam );

	if( inputPar == "omath" )
	{
		ConvertFromOM( inputFileName, &obj );
	}
	if( inputPar == "mathml" )
	{
		mmlparser.Pars( inputFileName );
	}
	std::string outputPar( outputParam );
	if( outputPar == "omath" )
	{
		if( inputPar == "omath" )
		{
			ConvertToOM( outputFileName, &obj );
		}
		if( inputPar == "mathml" )
		{
			ConvertToOM( outputFileName, mmlparser.GetData() );
		}
	}
    if( outputPar == "mathml" )
    {
        if( inputPar == "omath" )
        {
            mmlparser.SetData( &obj );
            mmlparser.Save( outputFileName );
        }
        if( inputPar == "mathml" )
            mmlparser.Save( outputFileName );
    }
}

//void MTRead( char* param, char* inputFileName, MathObj* obj ) 
//{
//	std::string par( param );
//	if( par == "omath" ) 
//    {
//		ConvertFromOM( inputFileName, obj );
//	}
//	if( par == "mathml" )
//	{
//		MathMLParser* A = new MathMLParser();
//		A->Pars( inputFileName );
//		obj = A->GetData( );
//	}
//	
//}

//void MTWrite( char* param, char* outputFileName, MathObj* obj ) 
//{
//	std::string par( param );
//	if( par == "omath" ) 
//    {
//		ConvertToOM( outputFileName, obj );
//	}
//}


int main( int argc, char* argv[] ) 
{
	if( argc != 5 ) {
		std::cout << "nevernoe kolichestvo parametrov!" << std::endl;
		system( "pause" );
		return 0;
	}
    MTConvert( argv[1], argv[2], argv[3], argv[4] );

	system( "pause" );
	return 0;
}


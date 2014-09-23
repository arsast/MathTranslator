// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

void MTRead(wchar_t* inputFileName, std::string& input) {
	
	if( inputFileName == NULL ) {
		std::cout << "Entre file name, please!" << std::endl;
		return;
	}
	
	std::ifstream someFile;
	someFile.open( inputFileName );
	std::string line;
	if ( someFile.is_open() )
	{
		while ( std::getline( someFile, line ) )
		{
			input = input + line;
		}
	}
	someFile.close();
};


extern void MTTranslate();
void MTWrite(wchar_t* outputFileName, wchar_t* output) {
	std::wfstream someFile;
	someFile.open(outputFileName);
	someFile << output;
	std::wcout << output << std::endl;
	someFile.close();
}


int wmain(int argc, wchar_t* argv[]) {

	if (argc != 5) {
		std::wcout << L"Неверное количество параметров!" << std::endl;
		return 0;
	}

	wchar_t* buffer = new wchar_t[1024];
	std::string input;
	MTRead(argv[2], input);
	MTWrite(argv[4], buffer);

	return 0;
}


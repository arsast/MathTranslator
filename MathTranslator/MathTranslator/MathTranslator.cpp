// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

void wMTRead(wchar_t* inputFileName, wchar_t* input) {
	

	std::wfstream someFile;
	someFile.open(inputFileName);
	someFile >> input;
	std::wcout << input << std::endl;
	someFile.close();
};

extern void Translate();
extern void Write();


int wmain(int argc, wchar_t* argv[]) {

	wchar_t* buffer = new wchar_t[1024];
	wMTRead(argv[1], buffer);

	return 0;
}


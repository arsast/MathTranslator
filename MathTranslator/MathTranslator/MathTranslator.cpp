// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

void MTRead(wchar_t* inputFileName, wchar_t* input) {
	

	std::wfstream someFile;
	someFile.open(inputFileName);
	someFile >> input;
	std::wcout << input << std::endl;
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
	MTRead(argv[2], buffer);
	MTWrite(argv[4], buffer);

	return 0;
}


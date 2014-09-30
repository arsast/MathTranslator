// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MathFormObj.h"
#include "ConvertOM.h"

void MTRead(char* param, char* inputFileName, MathObj* obj) {
	std::string par(param);
	if (par == "omath") {
		ConvertOM(inputFileName, obj);
	}
	
};


extern void MTTranslate();
void MTWrite(wchar_t* outputFileName, wchar_t* output) {
	std::wfstream someFile;
	someFile.open(outputFileName);
	someFile << output;
	std::wcout << output << std::endl;
	someFile.close();
}


int main(int argc, char* argv[]) {

	if (argc != 5) {
		std::cout << "Nevernoe kolichestvo parametrov!" << std::endl;
		system("pause");
		return 0;
	}
	
	FormulaObj obj(MAIN);
	MTRead(argv[1], argv[2], &obj);
	//MTWrite(argv[4], buffer);

	system("pause");
	return 0;
}


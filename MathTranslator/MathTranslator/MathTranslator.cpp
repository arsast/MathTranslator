// MathTranslator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MathFormObj.h"
#include "ConvertOM.h"

void MTRead(char* param, char* inputFileName, MathFormulaObj* obj) {
	std::string par(param);
	if (param == "omath") {
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
		std::wcout << "Неверное количество параметров!" << std::endl;
		return 0;
	}
	
	MathFormulaObj obj(MAIN);

	MTRead(argv[1], argv[2], &obj);
	//MTWrite(argv[4], buffer);

	return 0;
}


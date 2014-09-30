#include"MathML.h"

void MathMLParser::pars(string file){
	const char* str_file = file.c_str();
	TiXmlDocument doc(str_file);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", str_file	);
	}
	else
	{
		printf("Failed to load file \"%s\"\n", str_file);
	}
}
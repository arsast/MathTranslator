#include "stdafx.h"
#include "MathFormObj.h"
#include "targetver.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <string>
#include <iostream>
using namespace std;

class MathMLParser{
private:
	MathObj* root;
	MathObj* addElemToData(TiXmlElement* elem, MathObj* place);
public:
	void SetData(MathObj* newRoot){ root = newRoot; }
	MathObj* GetData(){ return root; }
	void Pars(char* file);
	void ConvertToMML(char* outputFile){};
	void ConvertFromMML(){};
};
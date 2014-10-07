#include "stdafx.h"
#include "MathFormObj.h"
#include "targetver.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <string>
#include <iostream>
#include <stack>
using namespace std;

class MathMLParser
{
private:
	MathObj* root;
	void addRowToData(TiXmlElement* elem, vector<MathObj*>::iterator place);
	void addArgToData( TiXmlElement* elem, vector<MathObj*>::iterator place );
	void test(MathObj* temp);
public:
	void SetData(MathObj* newRoot){ root = newRoot; }
	MathObj* GetData(){ return root; }
	void Pars(char* file);
	void ConvertToMML(char* outputFile){};
	void ConvertFromMML(){};
};
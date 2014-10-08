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
	void test(MathObj* temp);
public:
	void SetData(MathObj* newRoot){ root = newRoot; }
	MathObj* GetData(){ return root; }
	void Pars(char* file);
	void ConvertToMML(char* outputFile){};
	void ConvertFromMML(){};
};

class treeBuilder
{
	stack<TiXmlElement*> elements;
	stack<TNodeType> operations;
	stack<MathObj*> terms;
	bool prevIsTerm = false;
	bool uminus = false;
public:
	MathObj* GetObj( );
	void Push( TiXmlElement* elem );
};

int priority( TNodeType a );

bool compare( TNodeType a, TNodeType b );

TNodeType readBinarOperation( TiXmlElement* elem );

void addArgToData( TiXmlElement* elem, vector<MathObj*>::iterator place );

void addRowToData( TiXmlElement* elem, vector<MathObj*>::iterator place );
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
public:
	void SetData(MathObj* newRoot){ root = newRoot; }
	MathObj* GetData(){ return root; }
	void Pars(char* file);
    void Save( char* file );
};

class treeBuilder
{
	stack<TiXmlElement*> elements;
	stack<TNodeType> operations;
	stack<MathObj*> terms;
	bool prevIsTerm = false;
	bool uminus = false;
public:
	bool IsTree(){ return terms.top(); }
	MathObj* GetObj( );
	TiXmlElement* GetArg(){ return elements.top(); }
	void Push( TiXmlElement* elem );
};

int priority( TNodeType a );

bool compare( TNodeType a, TNodeType b );

TNodeType readBinarOperation( TiXmlElement* elem );

void addArgToData( TiXmlElement* elem, vector<MathObj*>::iterator place );

void addRowToData( TiXmlElement* elem, vector<MathObj*>::iterator place );

void saveTreeToXml( TiXmlElement*, MathObj*, bool ); // преобразует дерево разбора в xml документ, начиная с определенной вершины

void linkNewElem( TiXmlElement*, const char*, std::string* ); // добавляет в xml документе новый элемент с заданным тегом и значением
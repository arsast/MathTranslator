#include <vector>
#include <string>
#include "tinyxml.h"
#pragma once

enum TNodeType { UMINUS, PLUS, MINUS, EQUAL, MULT, DIV, ROOT, POW, MAIN, PARAM}; //Тип оператора в вершине дерева разбора формулы


//Основной тип формулы
class MathObj {
public:
	virtual ~MathObj() {};
};

class FormulaObj : public MathObj {
private:
	TNodeType type;
public:
	std::vector<MathObj*> params;

	FormulaObj(TNodeType);
	~FormulaObj();

	TNodeType GetType();
};

class ParamObj : public MathObj {
private:
	std::string val;
public:
	ParamObj(std::string);
	~ParamObj();

	std::string GetVal();
};



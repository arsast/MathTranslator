#include <vector>
#include <string>
#include "tinyxml.h"
#pragma once

enum TNodeType { UMINUS, PLUS, MINUS, EQUAL, MULT, DIV, ROOT, POW, ABS, SUM, PROD, MAIN, NOTYPE}; //Тип оператора в вершине дерева разбора формулы


//Основной тип формулы
class MathObj {
public:
	virtual ~MathObj() {};
};

class FormulaObj : public MathObj {
private:
	TNodeType type = NOTYPE;
public:
	std::vector<MathObj*> params;

	FormulaObj();
	FormulaObj(TNodeType);
	~FormulaObj();

	TNodeType GetType();
	void SetType(TNodeType);
};

class ParamObj : public MathObj {
private:
	std::string val;
public:
	ParamObj();
	ParamObj(std::string);
	~ParamObj();

	std::string GetVal();
};



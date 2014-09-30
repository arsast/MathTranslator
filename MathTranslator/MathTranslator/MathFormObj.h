#include <list>
#pragma once

enum TOperType { UMINUS, PLUS, MINUS, EQUAL, MULT, DIV, ROOT, POW, MAIN }; //Тип оператора в вершине дерева разбора формулы


//Основной тип формулы
class MathFormulaObj {
public:
	TOperType type;
	std::list<MathFormulaObj*> params;

	MathFormulaObj(TOperType);
	virtual ~MathFormulaObj() {}
};



// Автор: Николай Левшин 
// Назначение: Объявление класса MathObj, который используется как внутренний формат для конвертации данных

#include <vector>
#include <string>
#include "tinyxml.h"
#pragma once

enum TNodeType { 
	NT_UMINUS,  //унарный минус
	NT_PLUS,    //плюс
	NT_MINUS,   //минус
	NT_EQUAL,   //равно
	NT_MULTCM,  //коммутативное умножение
    NT_MULTNCM, //не коммутативное умножение
	NT_DIV,     //деление
	NT_ROOT,    //корень n-ой степени, первый потомок - степень, если потомок один, то по умолчанию квадратный корень
	NT_POW,     //возведение в степень, первый потомок - основание
	NT_ABS,     //взятие модуля
	NT_SUM,     //оператор суммирования
	NT_PROD,    //оператор перемножения, как П
	NT_PLUSMINUS,//плюсминус
 

	NT_MAIN,    //Идентификатор того, что это корень нашего MathObj
	NT_NOTYPE,  //Безтипная вершина
}; //Тип оператора в вершине дерева разбора формулы


//Основной тип формулы
class MathObj {
public:
	virtual ~MathObj() {};
};

class FormulaObj : public MathObj {
private:
	TNodeType type = NT_NOTYPE;
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
	void SetVal(std::string s){ val = s; }
	~ParamObj();

	std::string GetVal();
};



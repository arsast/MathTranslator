// Автор: Николай Левшин 
// Назначение: Инициализация класса MathObj, который используется как внутренний формат для конвертации данных

#include "MathFormObj.h"

FormulaObj::FormulaObj() {}

FormulaObj::FormulaObj( TNodeType t ) : type(t) 
{ 
	params.clear(); 
}

FormulaObj::~FormulaObj() 
{
	std::vector<MathObj*>::iterator it;
	for( it = params.begin(); it != params.end(); ++it ) 
	{
		delete *it;
	}
	params.clear();
}

TNodeType FormulaObj::GetType() 
{ 
	return type; 
}

void FormulaObj::SetType( TNodeType t ) 
{
	type = t;
}

ParamObj::ParamObj() {}

ParamObj::ParamObj( std::string s ) : val(s) {}

ParamObj::~ParamObj() {}

std::string ParamObj::GetVal() 
{ 
	return val; 
}
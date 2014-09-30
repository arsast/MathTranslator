#include "MathFormObj.h"


FormulaObj::FormulaObj(TNodeType t) : type(t) { 
	params.clear(); 
}

FormulaObj::~FormulaObj() {
	std::vector<MathObj*>::iterator it;
	for (it = params.begin(); it != params.end(); ++it) {
		delete *it;
	}
	params.clear();
}

TNodeType FormulaObj::GetType() { 
	return type; 
}

ParamObj::ParamObj(std::string s) : val(s) {}
ParamObj::~ParamObj() {}
std::string ParamObj::GetVal() { return val; }




#include <list>
#pragma once

enum TOperType { UMINUS, PLUS, MINUS, EQUAL, MULT, DIV, ROOT, POW, MAIN }; //��� ��������� � ������� ������ ������� �������


//�������� ��� �������
class MathFormulaObj {
public:
	TOperType type;
	std::list<MathFormulaObj*> params;

	MathFormulaObj(TOperType);
	virtual ~MathFormulaObj() {}
};



// �����: ������� ������ 
// ����������: ���������� ������ MathObj, ������� ������������ ��� ���������� ������ ��� ����������� ������

#include <vector>
#include <string>
#include "tinyxml.h"
#pragma once

enum TNodeType { 
	NT_UMINUS, 
	NT_PLUS, 
	NT_MINUS, 
	NT_EQUAL, 
	NT_MULT, 
	NT_DIV, 
	NT_ROOT, 
	NT_POW, 
	NT_ABS, 
	NT_SUM, 
	NT_PROD, 
	NT_PLUSMINUS, 

	NT_MAIN, 
	NT_NOTYPE,
}; //��� ��������� � ������� ������ ������� �������


//�������� ��� �������
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



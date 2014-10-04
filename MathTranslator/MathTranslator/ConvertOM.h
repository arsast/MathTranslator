// �����: ������� ������
// ����������: ���������� �������, �������������� ��� ����������� ����� ��������� OpenMath � MathObj

#include "MathFormObj.h"

void ConvertFromOM( char*, MathObj* ); // ������������ OpenMath � MathObj, �������� �� ���� ��� �����, ������ ������
void ConvertElemToObj( TiXmlElement*, MathObj* ); // ������������ � MathObj, �������� �� ���� ��������� �� ������� ������� DOM
void ConvertToOM( char*, MathObj* ); // ������������ MathObj � OpenMath, �������� �� ���� ��� �����, � ������� ����������
void ConvertObjToElem( TiXmlElement*, MathObj* ); // ������������ � DOM, �������� �� ���� ��������� �� ������� ������� MathObj
void SetFormulaElementAttribute( TiXmlElement*, const TNodeType ); // ������������� �������� DOM ������ ��������
void SetFormulaObjType( FormulaObj*, std::string*, std::string* ); // ������������� �������� FormulaObj ������ ���� � ����������� �� ��������� 
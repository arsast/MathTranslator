// �����: ������� ������
// ����������: ���������� �������, �������������� ��� ����������� ����� ��������� OpenMath � MathObj

#include "MathFormObj.h"
#include <map>

typedef std::map<std::string, std::pair<std::string, int>> IdCollection;

void ConvertFromOM( char*, MathObj* ); // ������������ OpenMath � MathObj, �������� �� ���� ��� �����, ������ ������
void ConvertElemToObj( IdCollection*, TiXmlElement*, MathObj* ); // ������������ � MathObj, �������� �� ���� ��������� �� ������� ������� DOM
void ConvertToOM( char*, MathObj* ); // ������������ MathObj � OpenMath, �������� �� ���� ��� �����, � ������� ����������
void ConvertObjToElem( TiXmlElement*, MathObj* ); // ������������ � DOM, �������� �� ���� ��������� �� ������� ������� MathObj
void SetFormulaElementAttribute( TiXmlElement*, const TNodeType ); // ������������� �������� DOM ������ ��������
void SetFormulaObjType( IdCollection*, FormulaObj*, std::string*, std::string* ); // ������������� �������� FormulaObj ������ ���� � ����������� �� ��������� 
void LoadIdTable( IdCollection* ); // ���������� ������� ����������
// �����: ������� ������
// ����������: ���������� �������, �������������� ��� ����������� ����� ��������� OpenMath � MathObj

#include "MathFormObj.h"
#include <map>

typedef std::map<std::string, std::pair<std::string, int>> IdCollection;
typedef std::map<int, std::pair<std::string, std::string>> AttrCollection;

void ConvertFromOM( char*, MathObj* ); // ������������ OpenMath � MathObj, �������� �� ���� ��� �����, ������ ������
void ConvertElemToObj( IdCollection*, TiXmlElement*, MathObj* ); // ������������ � MathObj, �������� �� ���� ��������� �� ������� ������� DOM
void ConvertToOM( char*, MathObj* ); // ������������ MathObj � OpenMath, �������� �� ���� ��� �����, � ������� ����������
void ConvertObjToElem( AttrCollection*, TiXmlElement*, MathObj* ); // ������������ � DOM, �������� �� ���� ��������� �� ������� ������� MathObj
void SetFormulaElementAttribute( AttrCollection*, TiXmlElement*, const TNodeType ); // ������������� �������� DOM ������ ��������
void SetFormulaObjType( IdCollection*, FormulaObj*, std::string*, std::string* ); // ������������� �������� FormulaObj ������ ���� � ����������� �� ��������� 
void LoadIdTable( IdCollection* ); // ���������� ������� ����������
void LoadAttrTable( AttrCollection* ); // ���������� ������� ���������
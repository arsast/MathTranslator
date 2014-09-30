/*
�����: ������� ������
����������: ������������� ������� ����������� �� ������� OpenMath � 
*/

#include "ConvertOM.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <sstream>

void ConvertElem(TiXmlElement* pElem, MathObj* obj) {
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) //����������� �� ���� ��������� ������ ����
	{
		const std::string pKey = pElem->Value(); //�������� ��� ������� �������
		const char *pText = pElem->GetText();//�������� ���������� ����, ���� ��� ����, ����� NULL

		TiXmlAttribute* pAttrib = pElem->FirstAttribute();//������� ������ ������� ����, ���� ���, �� NULL

		//----------------------------------------------------------------------------------------------------------------------------
		// ������ ��� ������ ��� �������� ����������� ������ ������ � �������� ����, ��� �� ����� �������� ��� ���������� �� ���������
		std::cout << pKey.c_str() << std::endl;
		if (pText) {
			std::cout << pText << std::endl;
		}
		while (pAttrib) {
			std::cout << pAttrib->Name() << " " << pAttrib->Value();
			pAttrib = pAttrib->Next();
		}
		std::cout << std::endl;
		//----------------------------------------------------------------------------------------------------------------------------

		if (pKey == "OMOBJ") {
			ConvertElem(pElem->FirstChildElement(), obj);
		}
		else if (pKey == "OMV") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj node(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(&node);
		}
	}

}

void ConvertOM(char* inputFileName, MathObj* obj) {

	TiXmlDocument doc;
	doc.LoadFile(inputFileName);

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	pElem = hDoc.FirstChildElement().Element();

	ConvertElem(pElem, obj);

	
}
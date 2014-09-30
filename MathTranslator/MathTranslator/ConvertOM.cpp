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
		// ������ ��� ������ ��� �������� ����������� ������ ������ � �������� ����, ��� �� ����� �������� ��� ���������� �� ���������!!!
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

		//���� ������
		if (pKey == "OMOBJ") {
			ConvertElem(pElem->FirstChildElement(), obj);
		}

		//���� ����������
		else if (pKey == "OMV") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj node(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(&node);
		}

		//���� �����
		else if (pKey == "OMI") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj node(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(&node);
		}

		//���� �����������, ���������� ������ ��������
		else if (pKey == "OMA") {
			ConvertElem(pElem->FirstChildElement(), obj);
		}

		//���� �������
		else if (pKey == "OMS") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			std::string name = pAttrib->Name();

			//������� �������, ���������� �� ��� ��������
			while (name != "name") {
				pAttrib = pAttrib->Next();
				name = pAttrib->Name();
			}

			FormulaObj node;

			std::string operand = pAttrib->Value(); // ��������� ��� ��������
			//���� ������� �����
			if (operand == "unary_minus") {
				node.SetType(UMINUS);
			}

			//���� ����
			else if (operand == "plus") {
				node.SetType(PLUS);
			}

			//���� �����
			else if (operand == "minus") {
				node.SetType(MINUS);
			}

			//���� ���������
			else if (operand == "times") {
				node.SetType(MULT);
			}

			//���� �������
			else if (operand == "divide") {
				node.SetType(DIV);
			}

			//���� ���������� � �������
			else if (operand == "power") {
				node.SetType(POW);
			}

			//���� ������
			else if (operand == "abs") {
				node.SetType(ABS);
			}

			//���� ������
			else if (operand == "root") {
				node.SetType(ROOT);
			}

			//���� ������������
			else if (operand == "sum") {
				node.SetType(SUM);
			}

			//���� ������������
			else if (operand == "product") {
				node.SetType(PROD);
			}

			//���� ���������� ������
			else {
				std::cerr << "Error: " << operand.c_str() << " unknown operand!" << std::endl;
			}


			((FormulaObj*)obj)->params.push_back(&node);
			ConvertElem(pElem->FirstChildElement(), &node);
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
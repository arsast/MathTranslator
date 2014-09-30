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
			std::cout << pAttrib->Name() << " " << pAttrib->Value() << " ";
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
			ParamObj* node = new ParamObj(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(node);
		}

		//���� �����
		else if (pKey == "OMI") {
			ParamObj* node = new ParamObj(pElem->GetText());
			((FormulaObj*)obj)->params.push_back(node);
		}

		//���� �����������, ���������� ������ ��������
		else if (pKey == "OMA") {
			FormulaObj* node = new FormulaObj();
			((FormulaObj*)obj)->params.push_back(node);
			ConvertElem(pElem->FirstChildElement(), node);
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

			std::string operand = pAttrib->Value(); // ��������� ��� ��������
			//���� ������� �����
			if (operand == "unary_minus") {
				((FormulaObj*)obj)->SetType(UMINUS);
			}

			//���� ����
			else if (operand == "plus") {
				((FormulaObj*)obj)->SetType(PLUS);
			}

			//���� �����
			else if (operand == "minus") {
				((FormulaObj*)obj)->SetType(MINUS);
			}

			//���� ���������
			else if (operand == "times") {
				((FormulaObj*)obj)->SetType(MULT);
			}

			//���� �������
			else if (operand == "divide") {
				((FormulaObj*)obj)->SetType(DIV);
			}

			//���� ���������� � �������
			else if (operand == "power") {
				((FormulaObj*)obj)->SetType(POW);
			}

			//���� ������
			else if (operand == "abs") {
				((FormulaObj*)obj)->SetType(ABS);
			}

			//���� ������
			else if (operand == "root") {
				((FormulaObj*)obj)->SetType(ROOT);
			}

			//���� ������������
			else if (operand == "sum") {
				((FormulaObj*)obj)->SetType(SUM);
			}

			//���� ������������
			else if (operand == "product") {
				((FormulaObj*)obj)->SetType(PROD);
			}

			//Ec�� ���������
			else if (operand == "eq") {
				((FormulaObj*)obj)->SetType(EQUAL);
			}

			//���� ���������
			else if (operand == "plusminus") {
				((FormulaObj*)obj)->SetType(PLUSMINUS);
			}
			//���� ���������� ������
			else {
				std::cerr << "Error: " << operand.c_str() << " unknown operand!" << std::endl;
			}
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
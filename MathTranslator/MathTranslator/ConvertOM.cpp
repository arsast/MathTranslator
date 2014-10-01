//�����: ������� ������
//����������: ������������� ������� ����������� ����� ��������� OpenMath � MathObj


#include "ConvertOM.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <sstream>

void ConvertFromOM(char* inputFileName, MathObj* obj) {

	TiXmlDocument doc;
	doc.LoadFile(inputFileName);

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	pElem = hDoc.FirstChildElement().Element();

	ConvertElemToObj(pElem, obj);
}

void ConvertElemToObj(TiXmlElement* pElem, MathObj* obj) {
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) //����������� �� ���� ��������� ������ ����
	{
		const std::string pKey = pElem->Value(); //�������� ��� ������� �������
		const char *pText = pElem->GetText();//�������� ���������� ����, ���� ��� ����, ����� NULL

		TiXmlAttribute* pAttrib = pElem->FirstAttribute();//������� ������ ������� ����, ���� ���, �� NULL


		/*
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
		*/


		//���� ������
		if (pKey == "OMOBJ") {
			ConvertElemToObj(pElem->FirstChildElement(), obj);
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
			ConvertElemToObj(pElem->FirstChildElement(), node);
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
				((FormulaObj*)obj)->SetType(NT_UMINUS);
			}

			//���� ����
			else if (operand == "plus") {
				((FormulaObj*)obj)->SetType(NT_PLUS);
			}

			//���� �����
			else if (operand == "minus") {
				((FormulaObj*)obj)->SetType(NT_MINUS);
			}

			//���� ���������
			else if (operand == "times") {
				((FormulaObj*)obj)->SetType(NT_MULT);
			}

			//���� �������
			else if (operand == "divide") {
				((FormulaObj*)obj)->SetType(NT_DIV);
			}

			//���� ���������� � �������
			else if (operand == "power") {
				((FormulaObj*)obj)->SetType(NT_POW);
			}

			//���� ������
			else if (operand == "abs") {
				((FormulaObj*)obj)->SetType(NT_ABS);
			}

			//���� ������
			else if (operand == "root") {
				((FormulaObj*)obj)->SetType(NT_ROOT);
			}

			//���� ������������
			else if (operand == "sum") {
				((FormulaObj*)obj)->SetType(NT_SUM);
			}

			//���� ������������
			else if (operand == "product") {
				((FormulaObj*)obj)->SetType(NT_PROD);
			}

			//Ec�� ���������
			else if (operand == "eq") {
				((FormulaObj*)obj)->SetType(NT_EQUAL);
			}

			//���� ���������
			else if (operand == "plusminus") {
				((FormulaObj*)obj)->SetType(NT_PLUSMINUS);
			}

			//���� ���������� ������
			else {
				std::cerr << "Error: " << operand.c_str() << " unknown operand!" << std::endl;
			}
		}
	}

}

void ConvertToOM(char* outputFileName, MathObj* obj) {
	TiXmlDocument doc;
	TiXmlElement * element = new TiXmlElement("OMOBJ");
	element->SetAttribute("xmlns", "http://www.openmath.org/OpenMath");
	doc.LinkEndChild(element);

	ConvertObjToElem(element, ((FormulaObj*)obj)->params[0]);


}

void ConvertObjToElem( TiXmlElement* pElem, MathObj* obj ) 
{
	std::string classType = typeid( *obj ).name();
	std::cout << classType.c_str() << std::endl;
}


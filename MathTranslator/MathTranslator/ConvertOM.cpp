//Автор: Николай Левшин
//Назначение: Инициализация методов конвертации между форматами OpenMath в MathObj


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
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) //пробегаемся по всем элементам одного слоя
	{
		const std::string pKey = pElem->Value(); //получаем тэг текущей вершины
		const char *pText = pElem->GetText();//получаем содержание тега, если оно есть, иначе NULL

		TiXmlAttribute* pAttrib = pElem->FirstAttribute();//получем первый атрибут тэга, если нет, то NULL


		/*
		//----------------------------------------------------------------------------------------------------------------------------
		// Данный код ТОЛЬКО ДЛЯ ПРОВЕРКИ корректного обхода дерева и проверки того, что мы умеем выжимать всю информацию из документа!!!
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


		//Если корень
		if (pKey == "OMOBJ") {
			ConvertElemToObj(pElem->FirstChildElement(), obj);
		}

		//Если переменная
		else if (pKey == "OMV") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj* node = new ParamObj(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(node);
		}

		//Если число
		else if (pKey == "OMI") {
			ParamObj* node = new ParamObj(pElem->GetText());
			((FormulaObj*)obj)->params.push_back(node);
		}

		//Если конструкция, содержащая массив операций
		else if (pKey == "OMA") {
			FormulaObj* node = new FormulaObj();
			((FormulaObj*)obj)->params.push_back(node);
			ConvertElemToObj(pElem->FirstChildElement(), node);
		}

		//Если операнд
		else if (pKey == "OMS") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			std::string name = pAttrib->Name();

			//Находим атрибут, отвечающий за тип операнда
			while (name != "name") {
				pAttrib = pAttrib->Next();
				name = pAttrib->Name();
			}

			std::string operand = pAttrib->Value(); // Проверяем тип операнда

			//Если унарный минус
			if (operand == "unary_minus") {
				((FormulaObj*)obj)->SetType(NT_UMINUS);
			}

			//Если плюс
			else if (operand == "plus") {
				((FormulaObj*)obj)->SetType(NT_PLUS);
			}

			//Если минус
			else if (operand == "minus") {
				((FormulaObj*)obj)->SetType(NT_MINUS);
			}

			//Если умножение
			else if (operand == "times") {
				((FormulaObj*)obj)->SetType(NT_MULT);
			}

			//Если деление
			else if (operand == "divide") {
				((FormulaObj*)obj)->SetType(NT_DIV);
			}

			//Если возведение в степень
			else if (operand == "power") {
				((FormulaObj*)obj)->SetType(NT_POW);
			}

			//Если модуль
			else if (operand == "abs") {
				((FormulaObj*)obj)->SetType(NT_ABS);
			}

			//Если корень
			else if (operand == "root") {
				((FormulaObj*)obj)->SetType(NT_ROOT);
			}

			//Если суммирование
			else if (operand == "sum") {
				((FormulaObj*)obj)->SetType(NT_SUM);
			}

			//Если перемножение
			else if (operand == "product") {
				((FormulaObj*)obj)->SetType(NT_PROD);
			}

			//Ecли равенство
			else if (operand == "eq") {
				((FormulaObj*)obj)->SetType(NT_EQUAL);
			}

			//Если плюсминус
			else if (operand == "plusminus") {
				((FormulaObj*)obj)->SetType(NT_PLUSMINUS);
			}

			//Если незнакомый символ
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


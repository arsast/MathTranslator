/*
Автор: Николай Левшин
Назначение: Инициализация методов конвертации из формата OpenMath в 
*/

#include "ConvertOM.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <sstream>

void ConvertElem(TiXmlElement* pElem, MathObj* obj) {
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) //пробегаемся по всем элементам одного слоя
	{
		const std::string pKey = pElem->Value(); //получаем тэг текущей вершины
		const char *pText = pElem->GetText();//получаем содержание тега, если оно есть, иначе NULL

		TiXmlAttribute* pAttrib = pElem->FirstAttribute();//получем первый атрибут тэга, если нет, то NULL

		//----------------------------------------------------------------------------------------------------------------------------
		// Данный код ТОЛЬКО ДЛЯ ПРОВЕРКИ корректного обхода дерева и проверки того, что мы умеем выжимать всю информацию из документа!!!
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

		//Если корень
		if (pKey == "OMOBJ") {
			ConvertElem(pElem->FirstChildElement(), obj);
		}

		//Если переменная
		else if (pKey == "OMV") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj node(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(&node);
		}

		//Если число
		else if (pKey == "OMI") {
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj node(pAttrib->Value());
			((FormulaObj*)obj)->params.push_back(&node);
		}

		//Если конструкция, содержащая массив операций
		else if (pKey == "OMA") {
			ConvertElem(pElem->FirstChildElement(), obj);
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

			FormulaObj node;

			std::string operand = pAttrib->Value(); // Проверяем тип операнда
			//Если унарный минус
			if (operand == "unary_minus") {
				node.SetType(UMINUS);
			}

			//Если плюс
			else if (operand == "plus") {
				node.SetType(PLUS);
			}

			//Если минус
			else if (operand == "minus") {
				node.SetType(MINUS);
			}

			//Если умножение
			else if (operand == "times") {
				node.SetType(MULT);
			}

			//Если деление
			else if (operand == "divide") {
				node.SetType(DIV);
			}

			//Если возведение в степень
			else if (operand == "power") {
				node.SetType(POW);
			}

			//Если модуль
			else if (operand == "abs") {
				node.SetType(ABS);
			}

			//Если корень
			else if (operand == "root") {
				node.SetType(ROOT);
			}

			//Если суммирование
			else if (operand == "sum") {
				node.SetType(SUM);
			}

			//Если перемножение
			else if (operand == "product") {
				node.SetType(PROD);
			}

			//Если незнакомый символ
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
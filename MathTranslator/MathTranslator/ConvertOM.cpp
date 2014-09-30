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
			std::cout << pAttrib->Name() << " " << pAttrib->Value() << " ";
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
			ConvertElem(pElem->FirstChildElement(), node);
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
				((FormulaObj*)obj)->SetType(UMINUS);
			}

			//Если плюс
			else if (operand == "plus") {
				((FormulaObj*)obj)->SetType(PLUS);
			}

			//Если минус
			else if (operand == "minus") {
				((FormulaObj*)obj)->SetType(MINUS);
			}

			//Если умножение
			else if (operand == "times") {
				((FormulaObj*)obj)->SetType(MULT);
			}

			//Если деление
			else if (operand == "divide") {
				((FormulaObj*)obj)->SetType(DIV);
			}

			//Если возведение в степень
			else if (operand == "power") {
				((FormulaObj*)obj)->SetType(POW);
			}

			//Если модуль
			else if (operand == "abs") {
				((FormulaObj*)obj)->SetType(ABS);
			}

			//Если корень
			else if (operand == "root") {
				((FormulaObj*)obj)->SetType(ROOT);
			}

			//Если суммирование
			else if (operand == "sum") {
				((FormulaObj*)obj)->SetType(SUM);
			}

			//Если перемножение
			else if (operand == "product") {
				((FormulaObj*)obj)->SetType(PROD);
			}

			//Ecли равенство
			else if (operand == "eq") {
				((FormulaObj*)obj)->SetType(EQUAL);
			}

			//Если плюсминус
			else if (operand == "plusminus") {
				((FormulaObj*)obj)->SetType(PLUSMINUS);
			}
			//Если незнакомый символ
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
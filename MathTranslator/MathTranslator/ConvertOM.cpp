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
		// Данный код только для проверки корректного обхода дерева и проверки того, что мы умеем выжимать всю информацию из документа
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
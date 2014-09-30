#include "MathFormObj.h"

void ConvertFromOM(char*, MathObj*); // Конвертирует OpenMath в MathObj, принимая на вход имя файла, откуда читаем
void ConvertElemToObj(TiXmlElement*, MathObj*); // Конвертирует в MathObj, принимая на вход указатель на текущий элемент DOM
void ConvertToOM(char*, MathObj*); // Конвертирует MathObj в OpenMath, принимая на вход имя файла, в который записываем
void ConvertObjToElem(TiXmlElement*, MathObj*); // Конвертирует в DOM, принимая на вход указатель на текущий элемент MathObj
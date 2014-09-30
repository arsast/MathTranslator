#include "MathFormObj.h"


void ConvertElem(TiXmlElement*, MathObj*); // Конвертирует в MathObj, принимая на вход указатель на текущий элемент DOM
void ConvertOM(char*, MathObj*); //Конвертирует в MathObj, принимая на вход название файла
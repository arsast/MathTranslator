// Автор: Николай Левшин
// Назначение: Объявление функций, использующихся для конвертации между форматами OpenMath и MathObj

#include "MathFormObj.h"
#include <map>

typedef std::map<std::string, std::pair<std::string, int>> IdCollection;
typedef std::map<int, std::pair<std::string, std::string>> AttrCollection;

void ConvertFromOM( char*, MathObj* ); // Конвертирует OpenMath в MathObj, принимая на вход имя файла, откуда читаем
void ConvertElemToObj( IdCollection*, TiXmlElement*, MathObj* ); // Конвертирует в MathObj, принимая на вход указатель на текущий элемент DOM
void ConvertToOM( char*, MathObj* ); // Конвертирует MathObj в OpenMath, принимая на вход имя файла, в который записываем
void ConvertObjToElem( AttrCollection*, TiXmlElement*, MathObj* ); // Конвертирует в DOM, принимая на вход указатель на текущий элемент MathObj
void SetFormulaElementAttribute( AttrCollection*, TiXmlElement*, const TNodeType ); // Устанавливает элементу DOM нужные атрибуты
void SetFormulaObjType( IdCollection*, FormulaObj*, std::string*, std::string* ); // Устанавливает элементу FormulaObj нужный флаг в зависимости от атрибутов 
void LoadIdTable( IdCollection* ); // Подгружает таблицу операторов
void LoadAttrTable( AttrCollection* ); // Подгружает таблицу атрибутов
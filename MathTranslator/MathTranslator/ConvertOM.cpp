// Автор: Николай Левшин
// Назначение: Инициализация функций, использующихся для конвертации между форматами OpenMath и MathObj

#include "ConvertOM.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

typedef std::map<std::string, std::pair<std::string, int>> IdCollection;

void LoadIdTable( IdCollection* ids )
{
    std::ifstream fin( "OMTable.txt" ); //  в данном файле содержится таблица соответствия идентификаторов операндам
    std::string type, contDict, name;
    int id;
    while( fin ) // считываем файл и заполняем контейнер данными из таблицы
    {
        fin >> id >> type >> contDict >> name;
        if( fin )
        {
            if( name == "times" )
            {
                if( contDict == "arith1" )
                {
                    ( *ids )[name + "1"] = std::make_pair( contDict, id );
                }
                else
                {
                    ( *ids )[name + "2"] = std::make_pair( contDict, id );
                }
            }
            else
            {
                ( *ids )[name] = std::make_pair( contDict, id );
            }
        }
    }
    fin.close( );
}

void ConvertFromOM( char* inputFileName, MathObj* obj ) 
{
   
    IdCollection ids;
    LoadIdTable( &ids );

	TiXmlDocument doc;
	doc.LoadFile( inputFileName );

	TiXmlHandle hDoc( &doc );
	TiXmlElement* pElem;

	pElem = hDoc.FirstChildElement().Element();

	ConvertElemToObj(&ids, pElem, obj );
}

void ConvertElemToObj( IdCollection* ids, TiXmlElement* pElem, MathObj* obj ) {
	for( pElem; pElem; pElem = pElem->NextSiblingElement() ) //пробегаемся по всем элементам одного слоя
	{
		const std::string pKey = pElem->Value(); //получаем тэг текущей вершины
		const char *pText = pElem->GetText(); //получаем содержание тега, если оно есть, иначе NULL

		//Если корень
		if( pKey == "OMOBJ" ) 
		{
			ConvertElemToObj( ids, pElem->FirstChildElement(), obj );
		}

		//Если переменная
		else if( pKey == "OMV" ) 
		{
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj* node = new ParamObj( pAttrib->Value() );
			((FormulaObj*)obj)->params.push_back( node );
		}

		//Если целое число
		else if( pKey == "OMI" ) 
		{
			ParamObj* node = new ParamObj( pElem->GetText() );
			((FormulaObj*)obj)->params.push_back( node );
		}

        //Если десятичное число
        else if( pKey == "OMF" )
        {
            TiXmlAttribute* pAttrib = pElem->FirstAttribute();
            ParamObj* node = new ParamObj( pAttrib->Value( ) );
            ( (FormulaObj*) obj )->params.push_back( node );

        }
		//Если конструкция, содержащая массив операций
		else if( pKey == "OMA" ) 
		{
			FormulaObj* node = new FormulaObj();
			((FormulaObj*)obj)->params.push_back( node );
			ConvertElemToObj( ids, pElem->FirstChildElement(), node );
		}

		//Если операнд
		else if( pKey == "OMS" ) 
		{
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			std::string name = pAttrib->Name(); // Тип атрибута

            //Находим атрибут, отвечающий за название словаря, содержащего наш атрибут
            while( name != "cd" )
            {
                pAttrib = pAttrib->Next( );
                name = pAttrib->Name( );
            }

            std::string cd = pAttrib->Value(); // получаем требуемый словарь

			//Находим атрибут, отвечающий за тип операнда
			while( name != "name" ) 
			{
				pAttrib = pAttrib->Next();
				name = pAttrib->Name();
			}

			std::string operand = pAttrib->Value(); // Получаем тип операнда
			SetFormulaObjType( ids, (FormulaObj*)obj, &cd, &operand );
		}
	}

}

void ConvertToOM( char* outputFileName, MathObj* obj ) 
{
	TiXmlDocument doc;
	TiXmlElement* element = new TiXmlElement( "OMOBJ" );
	element->SetAttribute( "xmlns", "http://www.openmath.org/OpenMath" );
	doc.LinkEndChild( element );

	ConvertObjToElem( element, ((FormulaObj*)obj)->params[0] );

    doc.SaveFile( outputFileName );
}

void ConvertObjToElem( TiXmlElement* pElem, MathObj* obj ) 
{
	std::string objType = typeid( *obj ).name();
	std::cout << objType.c_str() << std::endl;
	if( objType == "class FormulaObj" ) 
    {
		TiXmlElement* element = new TiXmlElement( "OMA" );
		pElem->LinkEndChild( element );

		TiXmlElement* formulaElement = new TiXmlElement( "OMS" );

		SetFormulaElementAttribute( formulaElement, ((FormulaObj*)obj)->GetType() );
		element->LinkEndChild(formulaElement);

		std::vector<MathObj*>::iterator it = ((FormulaObj*)obj)->params.begin();
		std::vector<MathObj*>::const_iterator itEnd = ((FormulaObj*)obj)->params.end();
		for( it; it != itEnd; ++it ) 
		{
			ConvertObjToElem( element, *it );
		}
    }
    else if( objType == "class ParamObj" )
    {
        std::string val = ( (ParamObj*) obj )->GetVal();
        if( val[0] > 47 && val[0] < 58 ) // Если это число
        {
            if( val.find( '.' ) != std::string::npos ) // И в нем есть точка
            {
                TiXmlElement* element = new TiXmlElement( "OMF" );
                element->SetAttribute( "dec", val.c_str() );
                pElem->LinkEndChild( element );
            }
            else
            {
                TiXmlElement* element = new TiXmlElement( "OMI" );
                TiXmlText * text = new TiXmlText( val.c_str() );
                element->LinkEndChild( text );
                pElem->LinkEndChild( element );
            }
        }
        else
        {
            TiXmlElement* element = new TiXmlElement( "OMV" );
            element->SetAttribute( "name", val.c_str( ) );
            pElem->LinkEndChild( element );
        }
    }
}

void SetFormulaElementAttribute( TiXmlElement* pElem, TNodeType type ) 
{
	switch ( type )
	{
	case NT_UMINUS:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "unary_minus" );
		break;
	case NT_PLUS:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "plus" );
		break;
	case NT_MINUS:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "minus" );
		break;
	case NT_EQUAL:
		pElem->SetAttribute( "cd", "relation1" );
		pElem->SetAttribute( "name", "eq" );
		break;
	case NT_MULTNCM:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "times" );
		break;
    case NT_MULTCM:
        pElem->SetAttribute( "cd", "arith2" );
        pElem->SetAttribute( "name", "times" );
        break;
	case NT_DIV:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "divide" );
		break;
	case NT_ROOT:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "root" );
		break;
	case NT_POW:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "power" );
		break;
	case NT_ABS:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "abs" );
		break;
	case NT_SUM:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "sum" );
		break;
	case NT_PROD:
		pElem->SetAttribute( "cd", "arith1" );
		pElem->SetAttribute( "name", "product" );
		break;
    case NT_INVERSE:
        pElem->SetAttribute( "cd", "arith2" );
        pElem->SetAttribute( "name", "inverse" );
        break;
    case NT_LOG:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "log" );
        break;
    case NT_LN:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "ln" );
        break;
    case NT_EXP:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "exp" );
        break;
    case NT_SIN:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "sin" );
        break;
    case NT_COS:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "cos" );
        break;
    case NT_SEC:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "sec" );
        break;
    case NT_CSC:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "csc" );
        break;
    case NT_TAN:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "tan" );
        break;
    case NT_COT:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "cot" );
        break;
    case NT_SINH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "sinh" );
        break;
    case NT_COSH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "cosh" );
        break;
    case NT_SECH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "SECH" );
        break;
    case NT_CSCH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "csch" );
        break;
    case NT_TANH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "tanh" );
        break;
    case NT_COTH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "coth" );
        break;
    case NT_ARCSIN:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arcsin" );
        break;
    case NT_ARCCOS:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arccos" );
        break;
    case NT_ARCSEC:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arcsec" );
        break;
    case NT_ARCCSC:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arccsc" );
        break;
    case NT_ARCTAN:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arctan" );
        break;
    case NT_ARCCOT:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arccot" );
        break;
    case NT_ARCSINH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arcsinh" );
        break;
    case NT_ARCCOSH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arccosh" );
        break;
    case NT_ARCSECH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arcsech" );
        break;
    case NT_ARCCSCH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arccsch" );
        break;
    case NT_ARCTANH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arctan" );
        break;
    case NT_ARCCOTH:
        pElem->SetAttribute( "cd", "transc1" );
        pElem->SetAttribute( "name", "arccoth" );
        break;
    case NT_PLUSMINUS:
        pElem->SetAttribute( "cd", "multiops" );
        pElem->SetAttribute( "name", "plusminus" );
        break;
	default:
		std::cerr << "Error: invalid type of Node in MathObj:" << type << std::endl;
	}
}

void SetFormulaObjType( IdCollection* ids, FormulaObj* obj, std::string* cd, std::string* operand)
{
    if( *operand == "times" )
    {
        if( *cd == "arith1" )
        {
            *operand += "1";
        }
        else
        {
            *operand += "2";
        }
    }

    obj->SetType( TNodeType( (*ids)[*operand].second ) );
    std::cout << *operand << " " << (*ids)[*operand].second << std::endl;
}


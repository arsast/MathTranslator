// �����: ������� ������
// ����������: ������������� �������, �������������� ��� ����������� ����� ��������� OpenMath � MathObj

#include "ConvertOM.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <sstream>

void ConvertFromOM( char* inputFileName, MathObj* obj ) 
{

	TiXmlDocument doc;
	doc.LoadFile( inputFileName );

	TiXmlHandle hDoc( &doc );
	TiXmlElement* pElem;

	pElem = hDoc.FirstChildElement().Element();

	ConvertElemToObj( pElem, obj );
}

void ConvertElemToObj( TiXmlElement* pElem, MathObj* obj ) {
	for( pElem; pElem; pElem = pElem->NextSiblingElement() ) //����������� �� ���� ��������� ������ ����
	{
		const std::string pKey = pElem->Value(); //�������� ��� ������� �������
		const char *pText = pElem->GetText();//�������� ���������� ����, ���� ��� ����, ����� NULL

		//���� ������
		if( pKey == "OMOBJ" ) 
		{
			ConvertElemToObj(pElem->FirstChildElement(), obj);
		}

		//���� ����������
		else if( pKey == "OMV" ) 
		{
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj* node = new ParamObj( pAttrib->Value() );
			((FormulaObj*)obj)->params.push_back( node );
		}

		//���� ����� �����
		else if( pKey == "OMI" ) 
		{
			ParamObj* node = new ParamObj( pElem->GetText() );
			((FormulaObj*)obj)->params.push_back( node );
		}

        //���� ���������� �����
        else if( pKey == "OMF" )
        {
            TiXmlAttribute* pAttrib = pElem->FirstAttribute();
            ParamObj* node = new ParamObj( pAttrib->Value( ) );
            ( (FormulaObj*) obj )->params.push_back( node );

        }
		//���� �����������, ���������� ������ ��������
		else if( pKey == "OMA" ) 
		{
			FormulaObj* node = new FormulaObj();
			((FormulaObj*)obj)->params.push_back( node );
			ConvertElemToObj( pElem->FirstChildElement(), node);
		}

		//���� �������
		else if( pKey == "OMS" ) 
		{
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			std::string name = pAttrib->Name(); // ��� ��������

            //������� �������, ���������� �� �������� �������, ����������� ��� �������
            while( name != "cd" )
            {
                pAttrib = pAttrib->Next( );
                name = pAttrib->Name( );
            }

            std::string cd = pAttrib->Value(); // �������� ��������� �������

			//������� �������, ���������� �� ��� ��������
			while( name != "name" ) 
			{
				pAttrib = pAttrib->Next();
				name = pAttrib->Name();
			}

			std::string operand = pAttrib->Value(); // �������� ��� ��������
			SetFormulaObjType((FormulaObj*)obj, &cd, &operand);
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
        if( val[0] > 47 && val[0] < 58 ) // ���� ��� �����
        {
            if( val.find( '.' ) != std::string::npos ) // � � ��� ���� �����
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
	case NT_PLUSMINUS:
		pElem->SetAttribute( "cd", "multiops" );
		pElem->SetAttribute( "name", "plusminus" );
		break;
	default:
		std::cerr << "Error: invalid type of Node in MathObj" << std::endl;
	}
}

void SetFormulaObjType( FormulaObj* obj, std::string* cd, std::string* operand)
{
	//���� ������� �����
	if (*operand == "unary_minus")
	{
		obj->SetType(NT_UMINUS);
	}

	//���� ����
	else if (*operand == "plus")
	{
		obj->SetType(NT_PLUS);
	}

	//���� �����
	else if (*operand == "minus")
	{
		obj->SetType(NT_MINUS);
	}

	//���� ���������
	else if (*operand == "times")
	{
        if( *cd == "arith1" ) { obj->SetType( NT_MULTNCM ); }
        else if( *cd == "arith2" ) { obj->SetType( NT_MULTCM ); }
        else std::cerr << "Error: Unknown param. cd: " << cd->c_str() << ", param: " << operand->c_str() << std::endl;
	}

	//���� �������
	else if (*operand == "divide")
	{
		obj->SetType(NT_DIV);
	}

	//���� ���������� � �������
	else if (*operand == "power")
	{
		obj->SetType(NT_POW);
	}

	//���� ������
	else if (*operand == "abs")
	{
		obj->SetType(NT_ABS);
	}

	//���� ������
	else if (*operand == "root")
	{
		obj->SetType(NT_ROOT);
	}

	//���� ������������
	else if (*operand == "sum")
	{
		obj->SetType(NT_SUM);
	}

	//���� ������������
	else if (*operand == "product")
	{
		obj->SetType(NT_PROD);
	}

	//Ec�� ���������
	else if (*operand == "eq")
	{
		obj->SetType(NT_EQUAL);
	}

	//���� ���������
	else if (*operand == "plusminus")
	{
		obj->SetType(NT_PLUSMINUS);
	}

	//���� ���������� ������
	else
	{
		std::cerr << "Error: Unknown param. cd: " << cd->c_str() << ", param: " << operand->c_str() << std::endl;
	}
}


// �����: ������� ������
// ����������: ������������� �������, �������������� ��� ����������� ����� ��������� OpenMath � MathObj

#include "ConvertOM.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


void LoadIdTable( IdCollection* ids )
{
    std::ifstream fin( "OMTable.txt" ); //  � ������ ����� ���������� ������� ������������ ��������� ��������� ���������������
    std::string type, contDict, name;
    int id;
    while( fin ) // ��������� ���� � ��������� ��������� ������� �� �������
    {
        fin >> id >> type >> contDict >> name;
        if( fin )
        {
            if( name == "times" ) // ��� ��� ��������� ���, �� ���� ��� ����� �������.
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
void LoadAttrTable( AttrCollection* attrs )
{
    std::ifstream fin( "OMTable.txt" ); //  � ������ ����� ���������� ������� ������������ ��������� ��������� ���������������
    std::string type, contDict, name;
    int id;
    while( fin ) // ��������� ���� � ��������� ��������� ������� �� �������
    {
        fin >> id >> type >> contDict >> name;
        if( fin )
        {
            ( *attrs )[id] = std::make_pair( contDict, name );
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
	for( pElem; pElem; pElem = pElem->NextSiblingElement() ) //����������� �� ���� ��������� ������ ����
	{
		const std::string pKey = pElem->Value(); //�������� ��� ������� �������
		const char *pText = pElem->GetText(); //�������� ���������� ����, ���� ��� ����, ����� NULL

		//���� ������
		if( pKey == "OMOBJ" ) 
		{
			ConvertElemToObj( ids, pElem->FirstChildElement(), obj );
		}

		//���� ����������
		else if( pKey == "OMV" ) 
		{
			TiXmlAttribute* pAttrib = pElem->FirstAttribute();
			ParamObj* node = new ParamObj( pAttrib->Value() );
            static_cast<FormulaObj*>( obj )->params.push_back( node );
		}

		//���� ����� �����
		else if( pKey == "OMI" ) 
		{
			ParamObj* node = new ParamObj( pElem->GetText() );
            static_cast<FormulaObj*>( obj )->params.push_back( node );
		}

        //���� ���������� �����
        else if( pKey == "OMF" )
        {
            TiXmlAttribute* pAttrib = pElem->FirstAttribute();
            ParamObj* node = new ParamObj( pAttrib->Value( ) );
            static_cast<FormulaObj*>( obj )->params.push_back( node );

        }
		//���� �����������, ���������� ������ ��������
		else if( pKey == "OMA" ) 
		{
			FormulaObj* node = new FormulaObj();
            static_cast<FormulaObj*>( obj )->params.push_back( node );
			ConvertElemToObj( ids, pElem->FirstChildElement(), node );
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
            SetFormulaObjType( ids, static_cast<FormulaObj*>( obj ), &cd, &operand );
		}
	}

}
void ConvertToOM( char* outputFileName, MathObj* obj ) 
{
	TiXmlDocument doc;
	TiXmlElement* element = new TiXmlElement( "OMOBJ" );
	element->SetAttribute( "xmlns", "http://www.openmath.org/OpenMath" );
	doc.LinkEndChild( element );

    AttrCollection attrs;
    LoadAttrTable( &attrs );
    ConvertObjToElem( &attrs, element, static_cast<FormulaObj*>( obj )->params[0] );

    doc.SaveFile( outputFileName );
}
void ConvertObjToElem( AttrCollection* attrs, TiXmlElement* pElem, MathObj* obj ) 
{
	std::string objType = typeid( *obj ).name();
	std::cout << objType.c_str() << std::endl;
	if( objType == "class FormulaObj" ) 
    {
		TiXmlElement* element = new TiXmlElement( "OMA" );
		pElem->LinkEndChild( element );

		TiXmlElement* formulaElement = new TiXmlElement( "OMS" );

		SetFormulaElementAttribute( attrs, formulaElement, ((FormulaObj*)obj)->GetType() );
		element->LinkEndChild(formulaElement);

		std::vector<MathObj*>::iterator it = ((FormulaObj*)obj)->params.begin();
		std::vector<MathObj*>::const_iterator itEnd = ((FormulaObj*)obj)->params.end();
		for( it; it != itEnd; ++it ) 
		{
			ConvertObjToElem( attrs, element, *it );
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
void SetFormulaElementAttribute( AttrCollection* attrs, TiXmlElement* pElem, TNodeType type ) 
{
    if( attrs->find( static_cast<int>( type ) ) != attrs->end() )
    {
        pElem->SetAttribute( "cd", (*attrs)[type].first.c_str() );
        pElem->SetAttribute( "name", (*attrs)[type].second.c_str() );
    }
    else
    {
		std::cerr << "Error: invalid type of Node in MathObj:" << type << std::endl;
	}
}
void SetFormulaObjType( IdCollection* ids, FormulaObj* obj, std::string* cd, std::string* operand)
{
    if( *operand == "times" )// ��� ��� ��������� ���, �� ����� ��� �������, ���� � ���� ��� � ������� LoadIdTable
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


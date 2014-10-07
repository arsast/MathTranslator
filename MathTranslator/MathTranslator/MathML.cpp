#include"MathML.h"

int priority( TNodeType a )
{
	switch( a )
	{
	case NT_PLUS:
		return 2;
		break;
	case NT_UMINUS:
		return 3;
		break;
	case NT_MINUS:
		return 2;
		break;
	case NT_MULTNCM:
		return 3;
		break;
	case NT_DIV:
		return 3;
		break;
	case NT_POW:
		return 4;
		break;
	case NT_ABS:
		return 4;
		break;
	case NT_ROOT:
		return 4;
		break;
	case NT_SUM:
		return 4;
		break;
	case NT_PROD:
		return 4;
		break;
	case NT_MULTCM:
		return 4;
		break;
	case NT_INVERSE:
		return 4;
		break;
	case NT_LOG:
		return 4;
		break;
	case NT_LN:
		return 4;
		break;
	case NT_EXP:
		return 4;
		break;
	case NT_SIN:
		return 4;
		break;
	case NT_COS:
		return 4;
		break;
	case NT_TAN:
		return 4;
		break;
	case NT_SEC:
		return 4;
		break;
	case NT_CSC:
		return 4;
		break;
	case NT_COT:
		return 4;
		break;
	case NT_SINH:
		return 4;
		break;
	case NT_COSH:
		return 4;
		break;
	case NT_TANH:
		return 4;
		break;
	case NT_SECH:
		return 4;
		break;
	case NT_CSCH:
		return 4;
		break;
	case NT_COTH:
		return 4;
		break;
	case NT_ARCSIN:
		return 4;
		break;
	case NT_ARCCOS:
		return 4;
		break;
	case NT_ARCTAN:
		return 4;
		break;
	case NT_ARCSEC:
		return 4;
		break;
	case NT_ARCCSC:
		return 4;
		break;
	case NT_ARCCOT:
		return 4;
		break;
	case NT_ARCSINH:
		return 4;
		break;
	case NT_ARCCOSH:
		return 4;
		break;
	case NT_ARCTANH:
		return 4;
		break;
	case NT_ARCSECH:
		return 4;
		break;
	case NT_ARCCSCH:
		return 4;
		break;
	case NT_ARCCOTH:
		return 4;
		break;
	case NT_EQUAL:
		return 1;
		break;
	case NT_LESS:
		return 1;
		break;
	case NT_GREAT:
		return 1;
		break;
	case NT_NEQUAL:
		return 1;
		break;
	case NT_LESSEQ:
		return 1;
		break;
	case NT_GREATEQ:
		return 1;
		break;
	case NT_APPROX:
		return 1;
		break;
	case NT_PLUSMINUS:
		return 2;
		break;
	case NT_MAIN:
		return 0;
		break;
	case NT_NOTYPE:
		return 0;
		break;
	default:
		return 0;
		break;
	}
}

bool compare( TNodeType a, TNodeType b )
{
	return priority( a ) > priority( b );
}

TNodeType readBinarOperation( TiXmlElement* elem )
{
	string id( elem->GetText( ) );
	if( id == "+" )
	{
		return NT_PLUS;
	}
	if( id == "-" )
	{
		return NT_MINUS;
	}
	if( id == "*" )
	{
		return NT_MULTCM;
	}
	if( ( int ) id[0] == ( int )'±' )
	{
		return NT_PLUSMINUS;
	}
	if( id == "=" )
	{
		return NT_EQUAL;
	}
	return NT_NOTYPE;
}

void processElem( stack<TiXmlElement*>* elements, TiXmlElement* elem )
{
}

void processOperation( stack<TNodeType>* operations, TiXmlElement* elem )
{
}

void MathMLParser::Pars(char* file)
{
	TiXmlDocument doc(file);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay)
	{
		cout << "Failed to load file\n";
	}
	TiXmlElement *elem = doc.FirstChildElement();
	elem = elem->FirstChildElement();
	root = new FormulaObj();
	((FormulaObj*)root)->SetType(NT_MAIN);
	((FormulaObj*)root)->params.push_back(NULL);
	vector<MathObj*>::iterator place = ((FormulaObj*)root)->params.begin();
	addArgToData(elem, place);
	//test(root);
}

void MathMLParser::addArgToData( TiXmlElement* elem, vector<MathObj*>::iterator place )
{
	if( elem->Value( ) == string( "msqrt" ) )
	{
		FormulaObj* child = new FormulaObj( );
		*place = child;
		child->SetType( NT_ROOT );
		child->params.push_back( NULL );
		vector<MathObj*>::iterator argPlace = child->params.begin( );
		elem = elem->FirstChildElement( );
		addArgToData( elem, argPlace );
		return;
	}

	if( elem->Value( ) == string( "msup" ) )
	{
		FormulaObj* child = new FormulaObj( );
		*place = child;
		child->SetType( NT_POW );
		child->params.push_back( NULL );
		child->params.push_back( NULL );
		vector<MathObj*>::iterator argPlace = child->params.begin( );
		elem = elem->FirstChildElement( );
		addArgToData( elem, argPlace );
		++argPlace;
		elem = elem->NextSiblingElement( );
		addArgToData( elem, argPlace );
		return;
	}
	if( elem->Value( ) == string( "mfrac" ) )
	{
		FormulaObj* child = new FormulaObj( );
		*place = child;
		child->SetType( NT_DIV );
		child->params.push_back( NULL );
		child->params.push_back( NULL );
		vector<MathObj*>::iterator argPlace = child->params.begin( );
		elem = elem->FirstChildElement( );
		addArgToData( elem, argPlace );
		++argPlace;
		elem = elem->NextSiblingElement( );
		addArgToData( elem, argPlace );
		return;
	}
	if( elem->Value() == string( "mi" ) || elem->Value() == string( "mn" ) )
	{
		ParamObj* arg = new ParamObj( );
		*place = arg;
		arg->SetVal( elem->GetText() );
		return;
	}
	if( elem->Value() == string( "mrow" ) )
	{
		TiXmlElement* childElem = elem->FirstChildElement( );
		addRowToData( childElem, place );
		return;
	}
}

void buildTree( TiXmlElement* elem, vector<MathObj*>::iterator place )
{
	stack<TiXmlElement*> elements;
	stack<TNodeType> operations;
	TiXmlElement* it = elem->FirstChildElement( );
	if( it->Value( ) == string( "mo" ) || it->Value( ) == string( "mrow" ) )
	{
		if( it->Value( ) == string( "mrow" ) )
		{
			if( it->FirstChildElement( )->GetText( ) == string( "-" ) )
			{

			}
		}
		else
		{
			if( it->GetText( ) == string( "-" ) )
			{

			}
		}
	}
	for( it; it; it = it->NextSiblingElement( ) )
	{
		if( it->Value( ) == string( "mo" ) )
		{
			operations.push( readBinarOperation( it ) );
		}
		else
		{
			elements.push( it );
		}
	}
}

void MathMLParser::addRowToData(TiXmlElement* elem, vector<MathObj*>::iterator place)
{
	if( elem->Value( ) == string( "mrow" ) || elem->Value( ) == string( "mo" ) )
	{
		if( elem->Value() == string( "mo" ) )
		{
			if( elem->GetText() == string( "-" ) && !elem->PreviousSibling() )
			{
				TiXmlNode* PreviousToParent = elem->Parent( )->PreviousSibling( );
				if( ( PreviousToParent && PreviousToParent->Value() == string( "mrow" ) ) || !PreviousToParent )
				{
					addRowToData( elem->NextSiblingElement(), place);
				}
			}
			//{
			//	FormulaObj* uMinus = new FormulaObj( );
			//	uMinus->SetType( NT_UMINUS );
			//	uMinus->params.push_back( NULL );
			//	vector<MathObj*>::iterator argUMinus = uMinus->params.begin( );
			//	elem = elem->NextSiblingElement();
			//	addArgToData( elem, argUMinus );
			//}
			FormulaObj* child = new FormulaObj( );
			*place = child;
			child->SetType( readBinarOperation( elem ) );
			child->params.push_back( NULL );
			child->params.push_back( NULL );
			vector<MathObj*>::iterator argPlace = child->params.begin( );
			TiXmlNode* prev = elem->PreviousSibling();
			if( prev )
			{
				addArgToData( prev->ToElement(), argPlace );
			}
			else
			{
				TiXmlNode* parent = elem->Parent( );
				addArgToData( parent->PreviousSibling()->ToElement(), argPlace );
			}
			++argPlace;
			TiXmlElement* nextElem = elem->NextSiblingElement();
			if( nextElem )
			{
				addRowToData( nextElem, argPlace );
			}
			else
			{
				TiXmlNode* parent = elem->Parent( );
				addRowToData(parent->NextSiblingElement(),place);
			}
		}
		else
		{
			addRowToData( elem->FirstChildElement( ), place );
			return;
		}
	}
	else
	{
		TiXmlElement* nextElem = elem->NextSiblingElement();
		if( nextElem )
		{
			if( nextElem->Value() != string("mo") )
			{
				FormulaObj* child = new FormulaObj( );
				*place = child;
				child->SetType( NT_MULTCM );
				child->params.push_back( NULL );
				child->params.push_back( NULL );
				vector<MathObj*>::iterator argPlace = child->params.begin( );
				addArgToData( elem, argPlace );
				++argPlace;
				addRowToData( nextElem, argPlace );
			}
			else
			{
				addRowToData( nextElem, place );
			}
		}
		else
		{
			addArgToData( elem, place );
		}
	}
	return;
}

void MathMLParser::test(MathObj* temp)
{
	std::string objType = typeid(temp).name();
	std::cout << objType.c_str() << " ";
	if (objType == "class FormulaObj")
	{
		std::vector<MathObj*>::iterator it;
		for (it = ((FormulaObj*)temp)->params.begin(); it != ((FormulaObj*)temp)->params.end(); ++it)
		{
			test(*it);
		}
	}
	else
	{
		cout << ((ParamObj*)temp)->GetVal();
	}
	cout << endl;
}
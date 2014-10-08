#include"MathML.h"

void treeBuilder::Push( TiXmlElement* elem )
{
	if( uminus )
	{
		if( elem->Value() != string( "mo" ) )
		{
			FormulaObj* temp = new FormulaObj();
			temp->SetType( NT_UMINUS );
			temp->params.push_back( NULL );
			addArgToData( elem, temp->params.begin() );
			terms.push( temp );
			prevIsTerm = true;
			uminus = false;
		}
		else
		{
			cout << "error reading unary minus!" << endl;
		}
	}
	else
	{
		bool invisibleMult = false;
		TNodeType operation;
		if( elem->Value() == string( "mo" ) )
		{
			if( prevIsTerm )
			{
				operation = readBinarOperation( elem );
				prevIsTerm = false;
			}
			else
			{
				if( elem->GetText() == string( "-" ) )
				{
					uminus = true;
				}
				else
				{
					cout << "error reading binary operation!" << endl;
				}
				return;
			}
		}
		else
		{
			if( prevIsTerm )
			{
				operation = NT_MULTCM;
				invisibleMult = true;
			}
			else
			{
				terms.push( NULL );
				elements.push( elem );
				prevIsTerm = true;
				return;
			}
		}
		while( operations.size() && !compare( operation, operations.top() ) )
		{
			FormulaObj* temp = new FormulaObj();
			temp->SetType( operations.top() );
			operations.pop();
			MathObj* secondArg = terms.top();
			terms.pop();
			MathObj* firstArg = terms.top();
			terms.pop();
			temp->params.push_back( firstArg );
			temp->params.push_back( secondArg );
			if( secondArg == NULL || firstArg == NULL )
			{
				vector<MathObj*>::iterator argPlace = temp->params.begin();
				++argPlace;
				if( secondArg == NULL )
				{
					addArgToData( elements.top(), argPlace );
					elements.pop();
				}
				--argPlace;
				if( firstArg == NULL )
				{
					addArgToData( elements.top(), argPlace );
					elements.pop();
				}
			}
			terms.push( temp );
		}
		operations.push(operation);
		if( invisibleMult )
		{
			terms.push( NULL );
			elements.push( elem );
		}
	}
}

MathObj* treeBuilder::GetObj()
{
	while( operations.size( ) )
	{
		FormulaObj* temp = new FormulaObj( );
		temp->SetType( operations.top( ) );
		operations.pop( );
		MathObj* secondArg = terms.top( );
		terms.pop( );
		MathObj* firstArg = terms.top( );
		terms.pop( );
		temp->params.push_back( firstArg );
		temp->params.push_back( secondArg );
		if( secondArg == NULL || firstArg == NULL )
		{
			vector<MathObj*>::iterator argPlace = temp->params.begin( );
			++argPlace;
			if( secondArg == NULL )
			{
				addArgToData( elements.top( ), argPlace );
				elements.pop( );
			}
			--argPlace;
			if( firstArg == NULL )
			{
				addArgToData( elements.top( ), argPlace );
				elements.pop( );
			}
		}
		terms.push( temp );
	}
	return terms.top();
}

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
	if( id == "<" )
	{
		return NT_LESS;
	}
	if( id == ">" )
	{
		return NT_GREAT;
	}
	if( id == "≈" )
	{
		return NT_APPROX;
	}
	if( id == "\leqslant" || id == "\leq" )
	{
		return NT_LESSEQ;
	}
	if( id == "\geqslant" || id == "\geq" )
	{
		return NT_GREATEQ;
	}
	if( id == "≠" )
	{
		return NT_NEQUAL;
	}
	return NT_NOTYPE;
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

void addArgToData( TiXmlElement* elem, vector<MathObj*>::iterator place )
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

void addRowToData( TiXmlElement* elem, vector<MathObj*>::iterator place )
{
	treeBuilder rowTree;
	for( elem; elem; elem = elem->NextSiblingElement() )
	{
		if( elem->Value( ) == string( "mrow" ) )
		{
			TiXmlElement* subElem = elem->FirstChildElement();
			for( subElem; subElem; subElem = subElem->NextSiblingElement() )
			{
				rowTree.Push( subElem );
			}
		}
		else
		{
			rowTree.Push( elem );
		}
	}
	*place = rowTree.GetObj();
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
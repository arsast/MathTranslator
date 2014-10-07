#include"MathML.h"

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
	addElemToData(elem, place);
	//test(root);
}

TNodeType MathMLParser::readBinarOperation(TiXmlElement* elem)
{
	string id(elem->GetText());
	if (id == "+")
	{
		return NT_PLUS;
	}
	if (id == "-")
	{
		return NT_MINUS;
	}
	if (id == "*")
	{
		return NT_MULTCM;
	}
	if ((int)id[0] == (int)'±')
	{
		return NT_PLUSMINUS;
	}
	if (id == "=")
	{
		return NT_EQUAL;
	}
	return NT_NOTYPE;
}

void MathMLParser::addElemToData(TiXmlElement* elem, vector<MathObj*>::iterator place)
{
	if (elem->Value() == string("mrow"))
	{
		elem = elem->FirstChildElement();
		addElemToData(elem, place);
		return;
	}
	if (elem->Value() == string("mi") || elem->Value() == string("mn"))
	{
		string id = elem->GetText();
		elem = elem->NextSiblingElement();
		if (elem == NULL)
		{
			ParamObj* child = new ParamObj();
			child->SetVal(id);
			*place = child;
		}
		else
		{
			FormulaObj* child = new FormulaObj();
			*place = child;
			ParamObj* firstArg = new ParamObj();
			firstArg->SetVal(id);
			child->params.push_back(firstArg);
			child->params.push_back(NULL);
			vector<MathObj*>::iterator secondArgPlace = child->params.end();
			--secondArgPlace;

			if (elem->Value() != string("mo"))
			{
				child->SetType(NT_MULTCM);
				addElemToData(elem, secondArgPlace);
			}
			else
			{
				TNodeType operation = readBinarOperation(elem);
				child->SetType(operation);
				TiXmlNode* parent = elem->Parent();
				elem = elem->NextSiblingElement();
				if (elem)
				{
					addElemToData(elem, secondArgPlace);
				}
				else
				{
					elem = parent->NextSiblingElement();
					if (elem)
					{
						addElemToData(elem, secondArgPlace);
						return;
					}
					return;
				}
			}
		}
		return;
	}
	//if (elem->Value() == "mo")
	//{
	//	FormulaObj* child = new FormulaObj();
	//	FormulaObj* arg = new FormulaObj();
	//	ParamObj* arg_val = new ParamObj();
	//	*place = child;
	//	if (elem->GetText() == "-")
	//	{
	//		arg->SetType(NT_UMINUS);
	//	}

	//	child->params.push_back(arg);
	//	vector<MathObj*>::iterator argPlace = child->params.begin();
	//}
	if (elem->Value() == string("msqrt"))
	{
		FormulaObj* child = new FormulaObj();
		*place = child;
		child->SetType(NT_ROOT);
		child->params.push_back(NULL);
		vector<MathObj*>::iterator argPlace = child->params.begin();
		elem = elem->FirstChildElement();
		addElemToData(elem, argPlace);
	}

	if (elem->Value() == string("msup"))
	{
		FormulaObj* child = new FormulaObj();
		*place = child;
		child->SetType(NT_POW);
		ParamObj* firstArg = new ParamObj();
		ParamObj* secondArg = new ParamObj();
		child->params.push_back(firstArg);
		child->params.push_back(secondArg);
		elem = elem->FirstChildElement();
		secondArg->SetVal(elem->GetText());
		elem = elem->NextSiblingElement();
		firstArg->SetVal(elem->GetText());
		return;
	}
	if (elem->Value() == string("mfrac"))
	{
		FormulaObj* child = new FormulaObj();
		*place = child;
		child->SetType(NT_DIV);
		child->params.push_back(NULL);
		vector<MathObj*>::iterator argPlace = child->params.begin();
		elem = elem->FirstChildElement();
		addElemToData(elem, argPlace);
		child->params.push_back(NULL);
		argPlace = child->params.end();
		--argPlace;
		elem = elem->NextSiblingElement();
		addElemToData(elem, argPlace);
		return;
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
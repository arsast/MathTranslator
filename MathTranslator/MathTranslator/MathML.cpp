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
	addElemToData(elem, root);
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
		return NT_MULT;
	}
	if ((int)id[0] == (int)'±')
	{
		return NT_PLUSMINUS;
	}
	if (id == "=")
	{
		return NT_EQUAL;
	}
}

void MathMLParser::addElemToData(TiXmlElement* elem, MathObj* place)
{
	if (elem->Value() == "mrow")
	{
		elem = elem->FirstChildElement();
		addElemToData(elem, place);
		return;
	}
	if (elem->Value() == "mi" || elem->Value() == "mn")
	{
		string id = elem->GetText();
		elem = elem->NextSiblingElement();
		if (elem == NULL)
		{
			place = new ParamObj();
			((ParamObj*)place)->SetVal(id);
		}
		else
		{
			place = new FormulaObj();
			ParamObj* firstArg = new ParamObj();
			firstArg->SetVal(id);
			((FormulaObj*)place)->params.push_back(firstArg);
			MathObj* secondArg;
			((FormulaObj*)place)->params.push_back(secondArg);

			if (elem->Value() != "mo")
			{
				((FormulaObj*)place)->SetType(NT_MULT);
				addElemToData(elem, secondArg);
			}
			else
			{
				TNodeType operation = readBinarOperation(elem);
				((FormulaObj*)place)->SetType(operation);
				elem = elem->NextSiblingElement();
				if (elem)
				{
					addElemToData(elem, secondArg);
				}
				else
				{
					TiXmlNode* parent = elem->Parent();
					elem = parent->NextSiblingElement();
					if (elem)
					{
						addElemToData(elem, secondArg);
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
	//	if (elem->GetText() == "-"){
	//		place = new FormulaObj();
	//		((FormulaObj*)place)->SetType(NT_UMINUS);
	//		MathObj* Arg;
	//		((FormulaObj*)place)->params.push_back(Arg);
	//		elem = elem->NextSiblingElement();
	//		if (elem)
	//		{
	//			addElemToData(elem, Arg);
	//		}
	//		else
	//		{
	//			TiXmlNode* parent = elem->Parent();
	//			elem = parent->NextSiblingElement();
	//			if (elem)
	//			{
	//				addElemToData(elem, Arg);
	//				return;
	//			}
	//			return;
	//		}

	//	}
	//}
	if (elem->Value() == "msup")
	{
		place = new FormulaObj();
		((FormulaObj*)place)->SetType(NT_POW);
		ParamObj* firstArg, *secondArg;

		((FormulaObj*)place)->params.push_back(firstArg);
		((FormulaObj*)place)->params.push_back(secondArg);
		elem = elem->FirstChildElement();
		secondArg->SetVal(elem->GetText());
		elem = elem->NextSiblingElement();
		firstArg->SetVal(elem->GetText());

		return;
	}
	if (elem->Value() == "mfrac")
	{
		place = new FormulaObj();
		((FormulaObj*)place)->SetType(NT_DIV);
		MathObj* firstArg, *secondArg;

		((FormulaObj*)place)->params.push_back(firstArg);
		elem = elem->FirstChildElement();
		addElemToData(elem, firstArg);

		((FormulaObj*)place)->params.push_back(secondArg);
		elem = elem->NextSiblingElement();
		addElemToData(elem, secondArg);

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
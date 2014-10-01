#include"MathML.h"

void MathMLParser::Pars(char* file){
	TiXmlDocument doc(file);
	bool loadOkay = doc.LoadFile();
	if (!loadOkay)
	{
		cout << "Failed to load file\n";
	}
	TiXmlElement *elem = doc.FirstChildElement();
	elem = elem->FirstChildElement();
	elem = elem->FirstChildElement();
	cout << elem->GetText() << endl;
}

MathObj* MathMLParser::addElemToData(TiXmlElement* elem, MathObj* place){
	MathObj* result = 0;
	if (elem->Value() == "mi" || elem->Value() == "mn" || elem->Value() == "mtext" || elem->Value() == "ms"){
		string id = elem->GetText();
		elem = elem->NextSiblingElement();
		if (elem){
			addElemToData(elem, place);
		}
		else{
			place = new ParamObj();
			((ParamObj*)place)->SetVal(id);
		}
	}
	if (elem->Value() == "mo"){

	}
	if (elem->Value() == "mfrac"){
		place = new FormulaObj();
		((FormulaObj*)place)->SetType(NT_DIV);
		MathObj* firstArg, *secondArg;

		((FormulaObj*)place)->params.push_back(firstArg);
		elem = elem->FirstChildElement();
		addElemToData(elem, firstArg);

		((FormulaObj*)place)->params.push_back(secondArg);
		elem = elem->NextSiblingElement();
		addElemToData(elem, secondArg);
	}
	return result;
}
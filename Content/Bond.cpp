#include "Chemistry.h"

using namespace Chemistry;

Bond::Bond(BondType type, Atom& first, Atom& second)
{
	this->type = type;
	this->first = &first;
	this->second = &second;

}

string Bond::toString() {
	string result = "";
	result += this->first->toString();
	result += bondTypeToStr(this->type);
	result += this->second->toString();
	return result;
}
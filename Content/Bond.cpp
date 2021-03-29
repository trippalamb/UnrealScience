#include "..\Headers\Chemistry.h"

using namespace Chemistry;

Bond::Bond(BondType type, Atom& first, Atom& second)
{
	this->Type = type;
	this->First = &first;
	this->Second = &second;

}

string Bond::ToString() {
	string result = "";
	result += this->First->ToString();
	result += BondTypeToStr(this->Type);
	result += this->Second->ToString();
	return result;
}
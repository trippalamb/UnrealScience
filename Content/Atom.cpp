#include "..\Headers\Chemistry.h"
#include <vector>

using namespace std;
using namespace Chemistry;

Atom::Atom() {}

Atom::Atom(AtomType type, double pos[3])
{
	this->Type = type;
	//this->Properties = AtomicProperties(type);
	this->State = AtomicState(pos);
}

void Atom::AddBond(Bond& bond) 
{
	this->Bonds.push_back(&bond);
}

double* Atom::GetPos()
{
	return this->State.Pos;
}

AtomType Atom::GetType()
{
	return this->Type;
}

string Atom::ToString()
{
	AtomNames names = AtomTypeToNames(this->Type);
	return names.Symbol;
}
#include "Chemistry.h"
#include <vector>

using namespace std;
using namespace Chemistry;

Atom::Atom() {}

Atom::Atom(AtomType type, double pos[3])
{
	this->type = type;
	this->properties = AtomicProperties(type);
	this->state = AtomicState(pos);
}

void Atom::addBond(Bond& bond) 
{
	this->bonds.push_back(&bond);
}

double* Atom::getPos()
{
	return this->state.pos;
}

string Atom::toString()
{
	return atomTypeToStr(this->type);
}
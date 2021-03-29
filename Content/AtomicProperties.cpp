#include "Chemistry.h"
#include <iostream>

using namespace std;
using namespace Chemistry;

AtomicProperties::AtomicProperties() {}

AtomicProperties::AtomicProperties(AtomType type) 
{
	switch (type) {
	case(AtomType::Hydrogen):
		this->construct(1.2);
		break;
	case(AtomType::Oxygen):
		this->construct(1.52);
		break;
	default:
		cout << "This atom type is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

void AtomicProperties::construct(double vdwRadius)
{
	this->vdwRadius = vdwRadius;
}

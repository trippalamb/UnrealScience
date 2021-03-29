#include "..\Headers\Chemistry.h"
#include <iostream>

using namespace std;
using namespace Chemistry;

AtomicProperties::AtomicProperties() {}

AtomicProperties::AtomicProperties(AtomType type) 
{
	switch (type) {
	case(AtomType::Hydrogen):
		this->Construct(.012);
		break;
	case(AtomType::Oxygen):
		this->Construct(0.0152);
		break;
	default:
		cout << "This atom type is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

void AtomicProperties::Construct(double vdwRadius)
{
	this->VdwRadius = vdwRadius;
}

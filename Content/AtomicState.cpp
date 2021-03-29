#include "Chemistry.h"

using namespace Chemistry;

AtomicState::AtomicState() {}

AtomicState::AtomicState(double pos[3])
{
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
}
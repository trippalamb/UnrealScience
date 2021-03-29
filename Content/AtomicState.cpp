#include "..\Headers\Chemistry.h"

using namespace Chemistry;

AtomicState::AtomicState() {}

AtomicState::AtomicState(double pos[3])
{
	this->Pos[0] = pos[0];
	this->Pos[1] = pos[1];
	this->Pos[2] = pos[2];
}
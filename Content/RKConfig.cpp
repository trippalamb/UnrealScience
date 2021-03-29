#include "../Headers/RK.h"
using namespace GenericRK;

RKConfig::RKConfig(RKMethods method, double minDh, double maxDh, double error, double safetyFactor)
{
	this->Method = method;
	this->MinDh = minDh;
	this->MaxDh = maxDh;
	this->Error = error;
	this->SafetyFactor = safetyFactor;
}

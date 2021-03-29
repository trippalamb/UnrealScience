#include "../Headers/RK.h"
using namespace GenericRK;

RKConfig::RKConfig(RKMethods method, double minDh, double maxDh, double error, double safetyFactor)
{
	this->method = method;
	this->minDh = minDh;
	this->maxDh = maxDh;
	this->error = error;
	this->safetyFactor = safetyFactor;
}

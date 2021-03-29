#include "../Headers/RK.h"
#include <cmath>
#include <limits>
#include <cstddef>
#include <string>

using namespace std;
using namespace GenericRK;

ODE::ODE()
{

}

ODE::ODE(int n, double e)
{
	this->K = new double[n];
	this->E = e;

    this->X0 = 0.0;
    this->DX0 = 0.0;
    this->F = 0.0;
    this->FS = 0.0;
    
}

ODE::~ODE()
{

}

double ODE::CalcH(double h1, double sf)
{
    double df0 = this->Calcd0(h1);
    double df1 = abs(F - FS);
    df1 = (df1 <= 0.0) ? DBL_MIN : df1;
    double power = (df0 >= df1) ? 0.2 : 0.25;

    return sf * h1 * pow(abs(df0 / df1), power);
}

double ODE::Calcd0(double h)
{
    return abs(this->E * (*this->DX * h));
}

void ODE::Connect(double& x, double& dx, string name)
{
    this->X = &x;
    this->DX = &dx;
    this->Name = name;
}
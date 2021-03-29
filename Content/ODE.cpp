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
	this->k = new double[n];
	this->e = e;

    this->x0 = 0.0;
    this->dx0 = 0.0;
    this->f = 0.0;
    this->fs = 0.0;
    
}

ODE::~ODE()
{

}

double ODE::calcH(double h1, double sf)
{
    double df0 = this->calcd0(h1);
    double df1 = abs(f - fs);
    df1 = (df1 <= 0.0) ? DBL_MIN : df1;
    double power = (df0 >= df1) ? 0.2 : 0.25;

    return sf * h1 * pow(abs(df0 / df1), power);
}

double ODE::calcd0(double h)
{
    return abs(this->e * (*this->dx * h));
}

void ODE::connect(double& x, double& dx, string name)
{
    this->x = &x;
    this->dx = &dx;
    this->name = name;
}
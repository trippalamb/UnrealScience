#include "../Headers/RK.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

using namespace GenericRK;

RK::RK(){}

RK::RK(int n, RKConfig config)
{
	this->construct(n, config.method, config.minDh, config.maxDh, config.error, config.safetyFactor);
}

RK::RK(int n, RKMethods method, double dh)
{
	this->n = n;
	this->dh = dh;
	this->minDh = dh;
	this->maxDh = dh;
	this->error = 0.0;
	this->sf = 0.0;
	this->method = method;

	this->constantsInit(method);

	if(this->embedded){
		cout << "The rk method must be explicit to use this constructor";
		exit(EXIT_FAILURE);
	}

	this->system = new ODE*[this->n];

	for (int i = 0; i < this->n; i++)
	{
		this->system[i] = new ODE(this->stepTotal, this->error);
	}
}

RK::RK(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor)
{
	this->construct(n, method, minDh, maxDh, error, safetyFactor);
}

RK::~RK()
{
	delete[] system;

	for (int i = 0; i < this->stepTotal; i++) {
		delete[] this->a[i];
	}
	delete[] this->a;
	delete[] this->b;
	delete[] this->bs;
	delete[] this->c;
}

void RK::construct(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor)
{
	this->n = n;
	this->dh = minDh;
	this->minDh = minDh;
	this->maxDh = maxDh;
	this->error = error;
	this->sf = safetyFactor;
	this->method = method;

	this->constantsInit(this->method);

	this->system = new ODE*[this->n];

	for (int i = 0; i < this->n; i++)
	{
		this->system[i] = new ODE(this->stepTotal, this->error);
	}
}

double RK::motion(ODESystem& system, double currH, double targetH) {
	double hLeft = targetH - currH;
	this->h = currH;

	if (!this->embedded) { this->dh = this->minDh; }

	while (hLeft > (1.0e-15 * this->dh))
	{
		this->init(this->h, min(this->dh, hLeft));

		for (int i = 1; i <= this->stepTotal; i++)
		{
			this->takeStep(i);
			system.updateODEs();
			this->updateK(i);
		}

		this->calcOrderPS();

		if (this->embedded)
		{
			hLeft = this->runAdaptiveStepSizeLogic(system, hLeft);
		}
		else
		{
			this->finish();
			system.updateODEs();
			hLeft -= this->dh;
		}
	}

	return targetH;
}

void RK::init(double h0, double hReq)
{
	this->h0 = h0;
	this->h = h0;
	this->dh = hReq;

	this->step = 1;

	for (int i = 0; i < this->n; i++)
	{
		this->system[i]->x0 = *this->system[i]->x;
		this->system[i]->dx0 = *this->system[i]->dx;

		fill_n(this->system[i]->k, this->n, 0.0);
		this->system[i]->f = 0.0;
		this->system[i]->fs = 0.0;
	}
}

void RK::takeStep(int step)
{
	this->step = step;
	this->h = this->h0 + this->c[step - 1] * this->dh;

	for (int i = 0; i < this->n; i++)
	{
		*this->system[i]->x = 0.0;
		for (int j = 0; j < this->step - 1; j++)
		{
			*this->system[i]->x += this->a[step - 1][j] * this->system[i]->k[j];
		}
		*this->system[i]->x = this->system[i]->x0 + (this->dh * (*this->system[i]->x));
	}
}

void RK::updateK(int step)
{
	for (int i = 0; i < this->n; i++)
	{
		this->system[i]->k[step - 1] = *this->system[i]->dx;
	}
}

void RK::calcOrderPS()
{
	for (int i = 0; i < this->n; i++)
	{
		this->system[i]->fs = 0.0;
		for (int j = 0; j < this->stepTotal; j++)
		{
			this->system[i]->fs += this->bs[j] * this->system[i]->k[j];
		}
		this->system[i]->fs = this->system[i]->x0 + (this->dh * this->system[i]->fs);
	}
}

void RK::calcOrderP()
{
	for (int i = 0; i < this->n; i++)
	{
		this->system[i]->f = 0.0;
		for (int j = 0; j < this->stepTotal; j++)
		{
			this->system[i]->f += this->b[j] * this->system[i]->k[j];
		}
		this->system[i]->f = this->system[i]->x0 + (this->dh * this->system[i]->f);
	}
}

double RK::runAdaptiveStepSizeLogic(ODESystem& system, double hLeft)
{
	this->calcOrderP();

	double lastH = this->dh;
	this->dh = this->calcMinHerr();

	if (this->dh < lastH)
	{
		this->reset();
		return hLeft;
	}
	else
	{
		this->finish();
		system.updateODEs();
		return hLeft - lastH;
	}
}

double RK::calcMinHerr()
{
	double* he = new double[this->n];
	for (int i = 0; i < this->n; i++)
	{
		he[i] = this->system[i]->calcH(this->dh, this->sf);
	}
	double hCurrent = *min_element(he, he+this->n);

	if (hCurrent > this->maxDh) { hCurrent = this->maxDh; }
	else if (hCurrent < this->minDh) { hCurrent = this->minDh; }

	return hCurrent;
}

void RK::finish()
{
	for (int i = 0; i < this->n; i++)
	{
		*this->system[i]->x = this->system[i]->fs;
	}
}

void RK::reset()
{
	this->h = this->h0;
	for (int i = 0; i < this->n; i++)
	{
		*this->system[i]->x = this->system[i]->x0;
		*this->system[i]->dx = this->system[i]->dx0;
	}
}

void RK::constantsInit(RKMethods method)
{
	switch (method)
	{
	case (RKMethods::Euler):
		this->constantsInit_euler();
		break;
	case(RKMethods::Midpoint):
		this->constantsInit_midpoint();
		break;
	case(RKMethods::Classic4th):
		this->constantsInit_classic4th();
		break;
	case(RKMethods::Fehlberg45):
		this->constantsInit_fehlberg45();
		break;
	case(RKMethods::CashKarp):
		this->constantsInit_cashkarp();
		break;
	default:
		cout << "This rk method is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

void RK::constantsInit_allocateSizeAndZeroOut(int n)
{
	this->a = new double* [n];
	for (int i = 0; i < n; i++) { this->a[i] = new double[n]; }

	this->b = new double[n];
	this->bs = new double[n];
	this->c = new double[n];

	for (int i = 0; i < n; i++) {
		this->b[i] = 0.0;
		this->bs[i] = 0.0;
		this->c[i] = 0.0;
		for (int j = 0; j < n; j++) {
			this->a[i][j] = 0.0;
		}
	}
}

void RK::constantsInit_euler() {

	this->stepTotal = 1;
	this->embedded = false;

	this->constantsInit_allocateSizeAndZeroOut(this->stepTotal);

	this->a[0][0] = 0.0;
	this->b[0] = 0.0;
	this->bs[0] = 1.0;
	this->c[0] = 0.0;

}

void RK::constantsInit_midpoint() {
	this->stepTotal = 2;
	this->embedded = false;

	this->constantsInit_allocateSizeAndZeroOut(this->stepTotal);

	this->a[0][0] = 0.0; 
	this->a[0][1] = 0.0;
	this->a[1][0] = 1.0 / 2.0; 
	this->a[1][1] = 0.0;

	this->b[0] = 0.0;
	this->b[1] = 0.0;

	this->bs[0] = 0.0; 
	this->bs[1] = 1.0;

	this->c[0] = 0.0;
	this->c[1] = 1.0 / 2.0;
}

void RK::constantsInit_classic4th() {
	
	this->stepTotal = 4;
	this->embedded = false;

	this->constantsInit_allocateSizeAndZeroOut(this->stepTotal);

	this->a[1][0] = 0.5;
	this->a[2][1] = 0.5;
	this->a[3][2] = 1.0;

	this->b[0] = 0.0;

	this->bs[0] = 1.0 / 6.0;
	this->bs[1] = 1.0 / 3.0;
	this->bs[2] = 1.0 / 3.0;
	this->bs[3] = 1.0 / 6.0;

	this->c[0] = 0.0;
	this->c[1] = 0.5;
	this->c[2] = 0.5;
	this->c[3] = 1.0;
}

void RK::constantsInit_fehlberg45() {

	this->stepTotal = 6;
	this->embedded = true;

	// k coefficients a[i,j]
	this->a[1][0] = 1.0 / 4.0;
	this->a[2][0] = 3.0 / 32.0;
	this->a[3][0] = 1932.0 / 2197.0;
	this->a[4][0] = 439.0 / 216.0;
	this->a[5][0] = -8.0 / 27.0;

	this->a[2][1] = 9.0 / 32.0;
	this->a[3][1] = -7200.0 / 2197.0;
	this->a[4][1] = -8.0;
	this->a[5][1] = 2.0;

	this->a[3][2] = 7296.0 / 2197.0;
	this->a[4][2] = -3680.0 / 513.0;
	this->a[5][2] = -3544.0 / 2565.0;

	this->a[4][3] = -845.0 / 4104.0;
	this->a[5][3] = 1859.0 / 4104.0;

	this->a[5][4] = -11.0 / 40.0;

	//initialize 5th order constants
	this->b[0] = 16.0 / 135.0;
	this->b[1] = 0.0;
	this->b[2] = 6656.0 / 12825.0;
	this->b[3] = 28561.0 / 56430.0;
	this->b[4] = -9.0 / 50.0;
	this->b[5] = 2.0 / 55.0;

	//initialize 4th order constants
	this->bs[0] = 25.0 / 216.0;
	this->bs[1] = 0.0;
	this->bs[2] = 1408.0 / 2565.0;
	this->bs[3] = 2197.0 / 4104.0;
	this->bs[4] = -1.0 / 5.0;
	this->bs[5] = 0.0;

	//initialize c coefficients
	this->c[0] = 0.0;
	this->c[1] = 1.0 / 4.0;
	this->c[2] = 3.0 / 8.0;
	this->c[3] = 12.0 / 13.0;
	this->c[4] = 1.0;
	this->c[5] = 1.0 / 2.0;

}

void RK::constantsInit_cashkarp() {
	this->stepTotal = 6;
	this->embedded = true;

	this->constantsInit_allocateSizeAndZeroOut(this->stepTotal);

	// k coefficients a[i,j]
	this->a[1][0] = 1.0 / 5.0;
	this->a[2][0] = 3.0 / 40.0;
	this->a[3][0] = 3.0 / 10.0;
	this->a[4][0] = -11.0 / 54.0;
	this->a[5][0] = 1631.0 / 55296.0;

	this->a[2][1] = 9.0 / 40.0;
	this->a[3][1] = -9.0 / 10.0;
	this->a[4][1] = 5.0 / 2.0;
	this->a[5][1] = 175.0 / 512.0;

	this->a[3][2] = 6.0 / 5.0;
	this->a[4][2] = -70.0 / 27.0;
	this->a[5][2] = 575.0 / 13824.0;

	this->a[4][3] = 35.0 / 27.0;
	this->a[5][3] = 44275.0 / 110592.0;
	this->a[5][4] = 253.0 / 4096.0;

	//initialize 5th order constants
	this->b[0] = 37.0 / 378.0;
	this->b[1] = 0.0;
	this->b[2] = 250.0 / 621.0;
	this->b[3] = 125.0 / 594.0;
	this->b[4] = 0.0;
	this->b[5] = 512.0 / 1771.0;

	//initialize 4th order constants
	this->bs[0] = 2825.0 / 27648.0;
	this->bs[1] = 0.0;
	this->bs[2] = 18575.0 / 48384.0;
	this->bs[3] = 13525.0 / 55296.0;
	this->bs[4] = 277.0 / 14336.0;
	this->bs[5] = 1.0 / 4.0;

	//initialize c coefficients
	this->c[0] = 0.0;
	this->c[1] = 1.0 / 5.0;
	this->c[2] = 3.0 / 10.0;
	this->c[3] = 3.0 / 5.0;
	this->c[4] = 1.0;
	this->c[5] = 7.0 / 8.0;

}

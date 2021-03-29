#include "../Headers/RK.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

using namespace GenericRK;

RK::RK(){}

RK::RK(int n, RKConfig config)
{
	this->Construct(n, config.Method, config.MinDh, config.MaxDh, config.Error, config.SafetyFactor);
}

RK::RK(int n, RKMethods method, double dh)
{
	this->N = n;
	this->DH = dh;
	this->MinDh = dh;
	this->MaxDh = dh;
	this->Error = 0.0;
	this->SF = 0.0;
	this->Method = method;

	this->ConstantsInit(method);

	if(this->Embedded){
		cout << "The rk method must be explicit to use this constructor";
		exit(EXIT_FAILURE);
	}

	this->System = new ODE*[this->N];

	for (int i = 0; i < this->N; i++)
	{
		this->System[i] = new ODE(this->StepTotal, this->Error);
	}
}

RK::RK(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor)
{
	this->Construct(n, method, minDh, maxDh, error, safetyFactor);
}

RK::~RK()
{
	delete[] this->System;

	for (int i = 0; i < this->StepTotal; i++) {
		delete[] this->A[i];
	}
	delete[] this->A;
	delete[] this->A;
	delete[] this->BS;
	delete[] this->C;
}

void RK::Construct(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor)
{
	this->N = n;
	this->DH = minDh;
	this->MinDh = minDh;
	this->MaxDh = maxDh;
	this->Error = error;
	this->SF = safetyFactor;
	this->Method = method;

	this->ConstantsInit(this->Method);

	this->System = new ODE*[this->N];

	for (int i = 0; i < this->N; i++)
	{
		this->System[i] = new ODE(this->StepTotal, this->Error);
	}
}

double RK::Motion(ODESystem& system, double currH, double targetH) {
	double hLeft = targetH - currH;
	this->H = currH;

	if (!this->Embedded) { this->DH = this->MinDh; }

	while (hLeft > (1.0e-15 * this->DH))
	{
		this->Init(this->H, min(this->DH, hLeft));

		for (int i = 1; i <= this->StepTotal; i++)
		{
			this->TakeStep(i);
			system.UpdateODEs();
			this->UpdateK(i);
		}

		this->CalcOrderPS();

		if (this->Embedded)
		{
			hLeft = this->RunAdaptiveStepSizeLogic(system, hLeft);
		}
		else
		{
			this->Finish();
			system.UpdateODEs();
			hLeft -= this->DH;
		}
	}

	return targetH;
}

void RK::Init(double h0, double hReq)
{
	this->H0 = h0;
	this->H = h0;
	this->DH = hReq;

	this->Step = 1;

	for (int i = 0; i < this->N; i++)
	{
		this->System[i]->X0 = *this->System[i]->X;
		this->System[i]->DX0 = *this->System[i]->DX;

		fill_n(this->System[i]->K, this->N, 0.0);
		this->System[i]->F = 0.0;
		this->System[i]->FS = 0.0;
	}
}

void RK::TakeStep(int step)
{
	this->Step = step;
	this->H = this->H0 + this->C[step - 1] * this->DH;

	for (int i = 0; i < this->N; i++)
	{
		*this->System[i]->X = 0.0;
		for (int j = 0; j < this->Step - 1; j++)
		{
			*this->System[i]->X += this->A[step - 1][j] * this->System[i]->K[j];
		}
		*this->System[i]->X = this->System[i]->X0 + (this->DH * (*this->System[i]->X));
	}
}

void RK::UpdateK(int step)
{
	for (int i = 0; i < this->N; i++)
	{
		this->System[i]->K[step - 1] = *this->System[i]->DX;
	}
}

void RK::CalcOrderPS()
{
	for (int i = 0; i < this->N; i++)
	{
		this->System[i]->FS = 0.0;
		for (int j = 0; j < this->StepTotal; j++)
		{
			this->System[i]->FS += this->BS[j] * this->System[i]->K[j];
		}
		this->System[i]->FS = this->System[i]->X0 + (this->DH * this->System[i]->FS);
	}
}

void RK::CalcOrderP()
{
	for (int i = 0; i < this->N; i++)
	{
		this->System[i]->F = 0.0;
		for (int j = 0; j < this->StepTotal; j++)
		{
			this->System[i]->F += this->B[j] * this->System[i]->K[j];
		}
		this->System[i]->F = this->System[i]->X0 + (this->DH * this->System[i]->F);
	}
}

double RK::RunAdaptiveStepSizeLogic(ODESystem& system, double hLeft)
{
	this->CalcOrderP();

	double lastH = this->DH;
	this->DH = this->CalcMinHerr();

	if (this->DH < lastH)
	{
		this->Reset();
		return hLeft;
	}
	else
	{
		this->Finish();
		system.UpdateODEs();
		return hLeft - lastH;
	}
}

double RK::CalcMinHerr()
{
	double* he = new double[this->N];
	for (int i = 0; i < this->N; i++)
	{
		he[i] = this->System[i]->CalcH(this->DH, this->SF);
	}
	double hCurrent = *min_element(he, he+this->N);

	if (hCurrent > this->MaxDh) { hCurrent = this->MaxDh; }
	else if (hCurrent < this->MinDh) { hCurrent = this->MinDh; }

	return hCurrent;
}

void RK::Finish()
{
	for (int i = 0; i < this->N; i++)
	{
		*this->System[i]->X = this->System[i]->FS;
	}
}

void RK::Reset()
{
	this->H = this->H0;
	for (int i = 0; i < this->N; i++)
	{
		*this->System[i]->X = this->System[i]->X0;
		*this->System[i]->DX = this->System[i]->DX0;
	}
}

void RK::ConstantsInit(RKMethods method)
{
	switch (method)
	{
	case (RKMethods::Euler):
		this->ConstantsInit_euler();
		break;
	case(RKMethods::Midpoint):
		this->ConstantsInit_midpoint();
		break;
	case(RKMethods::Classic4th):
		this->ConstantsInit_classic4th();
		break;
	case(RKMethods::Fehlberg45):
		this->ConstantsInit_fehlberg45();
		break;
	case(RKMethods::CashKarp):
		this->ConstantsInit_cashkarp();
		break;
	default:
		cout << "This rk method is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

void RK::ConstantsInit_allocateSizeAndZeroOut(int n)
{
	this->A = new double* [n];
	for (int i = 0; i < n; i++) { this->A[i] = new double[n]; }

	this->B = new double[n];
	this->BS = new double[n];
	this->C = new double[n];

	for (int i = 0; i < n; i++) {
		this->B[i] = 0.0;
		this->BS[i] = 0.0;
		this->C[i] = 0.0;
		for (int j = 0; j < n; j++) {
			this->A[i][j] = 0.0;
		}
	}
}

void RK::ConstantsInit_euler() {

	this->StepTotal = 1;
	this->Embedded = false;

	this->ConstantsInit_allocateSizeAndZeroOut(this->StepTotal);

	this->A[0][0] = 0.0;
	this->B[0] = 0.0;
	this->BS[0] = 1.0;
	this->C[0] = 0.0;

}

void RK::ConstantsInit_midpoint() {
	this->StepTotal = 2;
	this->Embedded = false;

	this->ConstantsInit_allocateSizeAndZeroOut(this->StepTotal);

	this->A[0][0] = 0.0; 
	this->A[0][1] = 0.0;
	this->A[1][0] = 1.0 / 2.0; 
	this->A[1][1] = 0.0;

	this->B[0] = 0.0;
	this->B[1] = 0.0;

	this->BS[0] = 0.0; 
	this->BS[1] = 1.0;

	this->C[0] = 0.0;
	this->C[1] = 1.0 / 2.0;
}

void RK::ConstantsInit_classic4th() {
	
	this->StepTotal = 4;
	this->Embedded = false;

	this->ConstantsInit_allocateSizeAndZeroOut(this->StepTotal);

	this->A[1][0] = 0.5;
	this->A[2][1] = 0.5;
	this->A[3][2] = 1.0;

	this->B[0] = 0.0;

	this->BS[0] = 1.0 / 6.0;
	this->BS[1] = 1.0 / 3.0;
	this->BS[2] = 1.0 / 3.0;
	this->BS[3] = 1.0 / 6.0;

	this->C[0] = 0.0;
	this->C[1] = 0.5;
	this->C[2] = 0.5;
	this->C[3] = 1.0;
}

void RK::ConstantsInit_fehlberg45() {

	this->StepTotal = 6;
	this->Embedded = true;

	// k coefficients a[i,j]
	this->A[1][0] = 1.0 / 4.0;
	this->A[2][0] = 3.0 / 32.0;
	this->A[3][0] = 1932.0 / 2197.0;
	this->A[4][0] = 439.0 / 216.0;
	this->A[5][0] = -8.0 / 27.0;

	this->A[2][1] = 9.0 / 32.0;
	this->A[3][1] = -7200.0 / 2197.0;
	this->A[4][1] = -8.0;
	this->A[5][1] = 2.0;

	this->A[3][2] = 7296.0 / 2197.0;
	this->A[4][2] = -3680.0 / 513.0;
	this->A[5][2] = -3544.0 / 2565.0;

	this->A[4][3] = -845.0 / 4104.0;
	this->A[5][3] = 1859.0 / 4104.0;

	this->A[5][4] = -11.0 / 40.0;

	//initialize 5th order constants
	this->B[0] = 16.0 / 135.0;
	this->B[1] = 0.0;
	this->B[2] = 6656.0 / 12825.0;
	this->B[3] = 28561.0 / 56430.0;
	this->B[4] = -9.0 / 50.0;
	this->B[5] = 2.0 / 55.0;

	//initialize 4th order constants
	this->BS[0] = 25.0 / 216.0;
	this->BS[1] = 0.0;
	this->BS[2] = 1408.0 / 2565.0;
	this->BS[3] = 2197.0 / 4104.0;
	this->BS[4] = -1.0 / 5.0;
	this->BS[5] = 0.0;

	//initialize c coefficients
	this->C[0] = 0.0;
	this->C[1] = 1.0 / 4.0;
	this->C[2] = 3.0 / 8.0;
	this->C[3] = 12.0 / 13.0;
	this->C[4] = 1.0;
	this->C[5] = 1.0 / 2.0;

}

void RK::ConstantsInit_cashkarp() {
	this->StepTotal = 6;
	this->Embedded = true;

	this->ConstantsInit_allocateSizeAndZeroOut(this->StepTotal);

	// k coefficients a[i,j]
	this->A[1][0] = 1.0 / 5.0;
	this->A[2][0] = 3.0 / 40.0;
	this->A[3][0] = 3.0 / 10.0;
	this->A[4][0] = -11.0 / 54.0;
	this->A[5][0] = 1631.0 / 55296.0;

	this->A[2][1] = 9.0 / 40.0;
	this->A[3][1] = -9.0 / 10.0;
	this->A[4][1] = 5.0 / 2.0;
	this->A[5][1] = 175.0 / 512.0;

	this->A[3][2] = 6.0 / 5.0;
	this->A[4][2] = -70.0 / 27.0;
	this->A[5][2] = 575.0 / 13824.0;

	this->A[4][3] = 35.0 / 27.0;
	this->A[5][3] = 44275.0 / 110592.0;
	this->A[5][4] = 253.0 / 4096.0;

	//initialize 5th order constants
	this->B[0] = 37.0 / 378.0;
	this->B[1] = 0.0;
	this->B[2] = 250.0 / 621.0;
	this->B[3] = 125.0 / 594.0;
	this->B[4] = 0.0;
	this->B[5] = 512.0 / 1771.0;

	//initialize 4th order constants
	this->BS[0] = 2825.0 / 27648.0;
	this->BS[1] = 0.0;
	this->BS[2] = 18575.0 / 48384.0;
	this->BS[3] = 13525.0 / 55296.0;
	this->BS[4] = 277.0 / 14336.0;
	this->BS[5] = 1.0 / 4.0;

	//initialize c coefficients
	this->C[0] = 0.0;
	this->C[1] = 1.0 / 5.0;
	this->C[2] = 3.0 / 10.0;
	this->C[3] = 3.0 / 5.0;
	this->C[4] = 1.0;
	this->C[5] = 7.0 / 8.0;

}

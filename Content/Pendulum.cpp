#include "../Headers/BasicODESystems.h"
#include <math.h>
using namespace BasicODESystems;

Pendulum::Pendulum(double t, double px, double vx, RKConfig config)
{
	this->T = t;
	this->Px = px;
	this->Vx = vx;
	this->Ax = 0.0;

	this->Px0 = px;
	this->Vx0 = vx;
	this->Omega = sqrt(2.0);
	this->Rk = new RK(2, config);
	this->OdeSetup();

}

void Pendulum::Timestep(double targetTime)
{
	this->T = this->Rk->Motion(*this, this->T, targetTime);
}

void Pendulum::SetToTruth(double targetTime)
{
	double *data = this->Solution(targetTime);
}

void Pendulum::OdeSetup()
{
	this->Rk->System[0]->Connect(this->Px, this->Vx, "position");
	this->Rk->System[1]->Connect(this->Vx, this->Ax, "velocity");
}

void Pendulum::UpdateODEs()
{
	this->Ax = -(pow(this->Omega, 2.0) * this->Px);
}

double* Pendulum::Solution(double time)
{
	double phi = atan2(-this->Vx0, this->Px0 * this->Omega);
	double amplitude = this->Px0 / cos(phi);

	double position = amplitude * cos(this->Omega * time + phi);
	double velocity = -this->Omega * amplitude * sin(this->Omega * time + phi);
	double acceleration = -(this->Omega * this->Omega) * amplitude * cos(this->Omega * time + phi);

	static double result[4] = { time, position, velocity, acceleration };
	return result;
}

double* Pendulum::GetState()
{
	static double result[4] = { this->T, this->Px, this->Vx, this->Ax };
	return result;
}

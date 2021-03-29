#include "../Headers/BasicODESystems.h"
#include <math.h>
using namespace BasicODESystems;

Pendulum::Pendulum(double t, double px, double vx, RKConfig config)
{
	this->t = t;
	this->px = px;
	this->vx = vx;
	this->ax = 0.0;

	this->px0 = px;
	this->vx0 = vx;
	this->omega = sqrt(2.0);
	this->rk = new RK(2, config);
	this->odeSetup();

}

void Pendulum::timestep(double targetTime)
{
	this->t = this->rk->motion(*this, this->t, targetTime);
}

void Pendulum::setToTruth(double targetTime)
{
	double *data = this->solution(targetTime);
}

void Pendulum::odeSetup()
{
	this->rk->system[0]->connect(this->px, this->vx, "position");
	this->rk->system[1]->connect(this->vx, this->ax, "velocity");
}

void Pendulum::updateODEs()
{
	this->ax = -(pow(this->omega, 2.0) * this->px);
}

double* Pendulum::solution(double time)
{
	double phi = atan2(-this->vx0, this->px0 * this->omega);
	double amplitude = this->px0 / cos(phi);

	double position = amplitude * cos(this->omega * time + phi);
	double velocity = -this->omega * amplitude * sin(this->omega * time + phi);
	double acceleration = -(this->omega * this->omega) * amplitude * cos(this->omega * time + phi);

	static double result[4] = { time, position, velocity, acceleration };
	return result;
}

double* Pendulum::getState()
{
	static double result[4] = { this->t, this->px, this->vx, this->ax };
	return result;
}

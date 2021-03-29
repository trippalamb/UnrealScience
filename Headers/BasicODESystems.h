#pragma once
#include "RK.h"

using namespace GenericRK;

namespace BasicODESystems
{
	class Pendulum : public ODESystem
	{
	public:
		Pendulum(double t, double px, double vx, RKConfig config);
		void timestep(double targetTime);
		void setToTruth(double targetTime);
		void odeSetup() override;
		void updateODEs() override;
		double* solution(double time);
		double* getState();
	private:
		double t;
		double px;
		double vx;
		double ax;
		double px0;
		double vx0;
		double omega;
		RK *rk;
	};
}


#pragma once
#include "RK.h"

using namespace GenericRK;

namespace BasicODESystems
{
	class Pendulum : public ODESystem
	{
	public:
		Pendulum(double t, double px, double vx, RKConfig config);
		void Timestep(double targetTime);
		void SetToTruth(double targetTime);
		void OdeSetup() override;
		void UpdateODEs() override;
		double* Solution(double time);
		double* GetState();
	private:
		double T;
		double Px;
		double Vx;
		double Ax;
		double Px0;
		double Vx0;
		double Omega;
		RK *Rk;
	};
}


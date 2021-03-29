#include <string>
using namespace std;
#pragma once

namespace GenericRK
{
	enum class RKMethods {Euler, Midpoint, Classic4th, Fehlberg45, CashKarp};

	struct RKConfig
	{
		RKMethods method = RKMethods::Midpoint; //method for rk to use
		double minDh = 0.1; //minimum allowable delta h
		double maxDh = 1.0; //maximum allowable delta h
		double error = 0.001; //target allowable error
		double safetyFactor = 0.9; //safety factor

		RKConfig(RKMethods method, double minDh, double maxDh, double error, double safetyFactor);
	};

	class ODE
	{
	public:
		string name;
		double* x; //value of ode
		double* dx; //derivative of x

		double x0; //holds inital value
		double dx0; //holds initial derivative value

		double f; //holds ode (p+1)th order value
		double fs; //holds ode pth order value
		double* k; //holds value for each step

		double e; //ode epsilon

		ODE();
		ODE(int n, double e);
		~ODE();
		double calcH(double h1, double sf);
		void connect(double& x, double& dx, string name);
	private:
		double calcd0(double h);
	};

	class ODESystem 
	{
	public:
		ODESystem() {};
		virtual void odeSetup() = 0;
		virtual void updateODEs() = 0;
	private:

	};

	class RK
	{
	public:
		ODE** system;
		RK();
		RK(int n, RKConfig config);
		RK(int n, RKMethods method, double dh);
		RK(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor);
		~RK();
		double motion(ODESystem& system, double currH, double targetH);
	private:
		
		RKMethods method;
		bool embedded;
		
		double h0;
		double h;
		double dh;
		
		double minDh;
		double maxDh;
		double error;
		double sf;

		int n;
		int step;
		int stepTotal;

		double** a;
		double* b;
		double* bs;
		double* c;

		void construct(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor);
		void init(double h0, double hReq);
		void takeStep(int step);
		void updateK(int step);
		void calcOrderPS();
		void calcOrderP();
		double runAdaptiveStepSizeLogic(ODESystem& system, double hLeft);
		double calcMinHerr();
		void finish();
		void reset();
		void constantsInit(RKMethods method);
		void constantsInit_allocateSizeAndZeroOut(int n);
		void constantsInit_euler();
		void constantsInit_midpoint();
		void constantsInit_classic4th();
		void constantsInit_fehlberg45();
		void constantsInit_cashkarp();

	};
}

#include <string>
using namespace std;
#pragma once

namespace GenericRK
{
	enum class RKMethods {Euler, Midpoint, Classic4th, Fehlberg45, CashKarp};

	struct RKConfig
	{
		RKMethods Method = RKMethods::Midpoint; //method for rk to use
		double MinDh = 0.1; //minimum allowable delta h
		double MaxDh = 1.0; //maximum allowable delta h
		double Error = 0.001; //target allowable error
		double SafetyFactor = 0.9; //safety factor

		RKConfig(RKMethods method, double minDh, double maxDh, double error, double safetyFactor);
	};

	class ODE
	{
	public:
		string Name;
		double* X; //value of ode
		double* DX; //derivative of x

		double X0; //holds inital value
		double DX0; //holds initial derivative value

		double F; //holds ode (p+1)th order value
		double FS; //holds ode pth order value
		double* K; //holds value for each step

		double E; //ode epsilon

		ODE();
		ODE(int n, double e);
		~ODE();
		double CalcH(double h1, double sf);
		void Connect(double& x, double& dx, string name);
	private:
		double Calcd0(double h);
	};

	class ODESystem 
	{
	public:
		ODESystem() {};
		virtual void OdeSetup() = 0;
		virtual void UpdateODEs() = 0;
	private:

	};

	class RK
	{
	public:
		ODE** System;
		RK();
		RK(int n, RKConfig config);
		RK(int n, RKMethods method, double dh);
		RK(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor);
		~RK();
		double Motion(ODESystem& system, double currH, double targetH);
	private:
		
		RKMethods Method;
		bool Embedded;
		
		double H0;
		double H;
		double DH;
		
		double MinDh;
		double MaxDh;
		double Error;
		double SF;

		int N;
		int Step;
		int StepTotal;

		double** A;
		double* B;
		double* BS;
		double* C;

		void Construct(int n, RKMethods method, double minDh, double maxDh, double error, double safetyFactor);
		void Init(double h0, double hReq);
		void TakeStep(int step);
		void UpdateK(int step);
		void CalcOrderPS();
		void CalcOrderP();
		double RunAdaptiveStepSizeLogic(ODESystem& system, double hLeft);
		double CalcMinHerr();
		void Finish();
		void Reset();
		void ConstantsInit(RKMethods method);
		void ConstantsInit_allocateSizeAndZeroOut(int n);
		void ConstantsInit_euler();
		void ConstantsInit_midpoint();
		void ConstantsInit_classic4th();
		void ConstantsInit_fehlberg45();
		void ConstantsInit_cashkarp();

	};
}

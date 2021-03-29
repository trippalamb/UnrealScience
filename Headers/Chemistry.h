#pragma once
#include <string>
#include <vector>

using namespace std;

namespace Chemistry
{
	enum class AtomType { Hydrogen, Helium, Lithium, Beryllium, Boron, Carbon, Nitrogen, Oxygen, Flourine, Neon };
	enum class BondType { Single, Double, Triple };

	double GetAtomicRadius(AtomType type);
	AtomType StrToAtomType(string str);
	string AtomTypeToStr(AtomType type);


	BondType StrToBondType(string str);
	string BondTypeToStr(BondType type);


	class Bond;

	//class AtomFactory
	//{
	//public:
	//	static Atom buildAtom(AtomType type);
	//};

	struct AtomicProperties
	{
		double VdwRadius = -1.0; //[angstroms] Van der Waals radius

		AtomicProperties();
		AtomicProperties(AtomType type);
	private:
		void Construct(double vdwRadius);
	};

	struct AtomicState
	{
		double Pos[3];
		AtomicState();
		AtomicState(double pos[3]);
	};



	class Atom
	{
	public:
		Atom();
		Atom(AtomType type, double pos[3]);
		void AddBond(Bond& bond);
		double* GetPos();
		AtomType GetType();
		string ToString();
	private:
		AtomType Type;
		AtomicState State;
		vector<Bond *> Bonds;
	};

	class Bond
	{
	public:
		Bond(BondType type, Atom& first, Atom& second);
		string ToString();
	private:
		BondType Type;
		Atom* First;
		Atom* Second;
	};

	class Molecule
	{
	public:
		Molecule(string filename);
		double* GetAtomPos(int i);
		AtomType GetAtomType(int i);
		int GetNumOfAtoms();
		string ToString();
	private:
		string Filename;
		string Name;
		string Timestep;
		string Comment;
		Atom** Atoms;
		Bond** Bonds;
		int N_Atoms;
		int N_Bonds;

		void ParseFile(string filename);
		void ParseSDF(string filename);
		void ParseSDF_header(ifstream* sdf);
		void ParseSDF_count(ifstream* sdf);
		void ParseSDF_atoms(ifstream* sdf);
		void ParseSDF_bonds(ifstream* sdf);
	};
}

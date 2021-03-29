#pragma once
#include <string>
#include <vector>

using namespace std;

namespace Chemistry
{
	enum class AtomType { Hydrogen, Helium, Lithium, Beryllium, Boron, Carbon, Nitrogen, Oxygen, Flourine, Neon };
	enum class BondType { Single, Double, Triple };

	AtomType strToAtomType(string str);
	BondType strToBondType(string str);
	string atomTypeToStr(AtomType type);
	string bondTypeToStr(BondType type);


	class Bond;

	//class AtomFactory
	//{
	//public:
	//	static Atom buildAtom(AtomType type);
	//};

	struct AtomicProperties
	{
		double vdwRadius = -1.0; //[angstroms] Van der Waals radius

		AtomicProperties();
		AtomicProperties(AtomType type);
	private:
		void construct(double vdwRadius);
	};

	struct AtomicState
	{
		double pos[3];
		AtomicState();
		AtomicState(double pos[3]);
	};



	class Atom
	{
	public:
		Atom();
		Atom(AtomType type, double pos[3]);
		void addBond(Bond& bond);
		double* getPos();
		string toString();
	private:
		AtomType type;
		AtomicProperties properties;
		AtomicState state;
		vector<Bond *> bonds;
	};

	class Bond
	{
	public:
		Bond(BondType type, Atom& first, Atom& second);
		string toString();
	private:
		BondType type;
		Atom* first;
		Atom* second;
	};

	class Molecule
	{
	public:
		Molecule(string filename);
		double* getAtomPos(int i);
		int getNumOfAtoms();
		string toString();
	private:
		string filename;
		string name;
		string timestep;
		string comment;
		Atom** atoms;
		Bond** bonds;
		int nAtoms;
		int nBonds;

		void parseFile(string filename);
		void parseSDF(string filename);
		void parseSDF_header(ifstream* sdf);
		void parseSDF_count(ifstream* sdf);
		void parseSDF_atoms(ifstream* sdf);
		void parseSDF_bonds(ifstream* sdf);
	};
}

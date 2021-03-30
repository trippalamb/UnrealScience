#include "..\Headers\Chemistry.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Chemistry;

string getExtension(string fn);
string removeLeadingWhiteSpace(string in);


Molecule::Molecule(string filename)
{
	this->Filename = filename;
	this->ParseFile(filename);
}

double* Molecule::GetAtomPos(int i)
{
	return this->Atoms[i]->GetPos();
}

AtomType Molecule::GetAtomType(int i)
{
	return this->Atoms[i]->GetType();
}

int Molecule::GetNumOfAtoms()
{
	return this->N_Atoms;
}

string Molecule::ToString()
{
	string result = "";

	for (int i = 0; i < this->N_Atoms; i++)
	{
		result += this->Atoms[i]->ToString();
	}

	for (int i = 0; i < this->N_Bonds; i++)
	{
		result += this->Bonds[i]->ToString();
	}

	return result;
}

void Molecule::ParseFile(string filename)
{

	string ext = getExtension(filename);

	if (ext.compare("sdf") == 0)
	{
		this->ParseSDF(filename);
	}
	else 
	{
		cout << "This extension type is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

void Molecule::ParseSDF(string filename) 
{
	ifstream* sdf = new ifstream(filename);

	this->ParseSDF_header(sdf);
	this->ParseSDF_count(sdf);
	this->ParseSDF_atoms(sdf);
	this->ParseSDF_bonds(sdf);

}

void Molecule::ParseSDF_header(ifstream* sdf) 
{
	getline(*sdf, this->Name);
	getline(*sdf, this->Timestep);
	getline(*sdf, this->Comment);
}

void Molecule::ParseSDF_count(ifstream* sdf)
{
	string line;
	getline(*sdf, line);
	this->N_Atoms = stoi(line.substr(0, 3));
	this->N_Bonds = stoi(line.substr(3, 3));

	this->Atoms = new Atom * [N_Atoms];
	this->Bonds = new Bond * [N_Bonds];
}

void Molecule::ParseSDF_atoms(ifstream* sdf)
{
	string line;

	for (int i = 0; i < this->N_Atoms; i++)
	{
		double pos[3];
		string element;

		getline(*sdf, line);
		pos[0] = stod(line.substr(0, 10));
		pos[1] = stod(line.substr(10, 10));
		pos[2] = stod(line.substr(20, 10));
		element = removeLeadingWhiteSpace(line.substr(30, 2));

		AtomType type = StrToAtomType(element);
		this->Atoms[i] = new Atom(type, pos);

	}
}

void Molecule::ParseSDF_bonds(ifstream* sdf)
{
	string line;

	for (int i = 0; i < this->N_Bonds; i++)
	{

		getline(*sdf, line);
		int atom1 = stoi(line.substr(0, 3))-1;
		int atom2 = stoi(line.substr(3, 3))-1;
		string order = line.substr(6, 3);

		BondType type = StrToBondType(order);
		this->Bonds[i] = new Bond(type, *this->Atoms[atom1], *this->Atoms[atom2]);

	}
}

string getExtension(string fn)
{
	int i = fn.find_last_of(".");
	if (i == -1)
	{
		cout << "The filename must have an extension.";
		exit(EXIT_FAILURE);
	}

	return fn.substr(i + 1);
}

string removeLeadingWhiteSpace(string in)
{
	const string WHITESPACE = " \n\r\t\f\v";
	int start = in.find_first_not_of(WHITESPACE);
	return in.substr(start);
}



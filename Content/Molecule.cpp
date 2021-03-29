#include "Chemistry.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Chemistry;

string getExtension(string fn);

Molecule::Molecule(string filename)
{
	this->filename = filename;
	this->parseFile(filename);
}

double* Molecule::getAtomPos(int i)
{
	return this->atoms[i]->getPos();
}

int Molecule::getNumOfAtoms()
{
	return this->nAtoms;
}

string Molecule::toString()
{
	string result = "";

	for (int i = 0; i < this->nAtoms; i++)
	{
		result += this->atoms[i]->toString();
	}

	for (int i = 0; i < this->nBonds; i++)
	{
		result += this->bonds[i]->toString();
	}

	return result;
}

void Molecule::parseFile(string filename)
{

	string ext = getExtension(filename);

	if (ext.compare("sdf") == 0)
	{
		this->parseSDF(filename);
	}
	else 
	{
		cout << "This extension type is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

void Molecule::parseSDF(string filename) 
{
	ifstream* sdf = new ifstream(filename);

	this->parseSDF_header(sdf);
	this->parseSDF_count(sdf);
	this->parseSDF_atoms(sdf);
	this->parseSDF_bonds(sdf);

}

void Molecule::parseSDF_header(ifstream* sdf) 
{
	getline(*sdf, this->name);
	getline(*sdf, this->timestep);
	getline(*sdf, this->comment);
}

void Molecule::parseSDF_count(ifstream* sdf)
{
	string line;
	getline(*sdf, line);
	this->nAtoms = stoi(line.substr(0, 3));
	this->nBonds = stoi(line.substr(3, 3));

	this->atoms = new Atom * [nAtoms];
	this->bonds = new Bond * [nBonds];
}

void Molecule::parseSDF_atoms(ifstream* sdf)
{
	string line;

	for (int i = 0; i < this->nAtoms; i++)
	{
		double pos[3];
		string element;

		getline(*sdf, line);
		pos[0] = stod(line.substr(0, 10));
		pos[1] = stod(line.substr(10, 10));
		pos[2] = stod(line.substr(20, 10));
		element = line.substr(30, 2);

		AtomType type = strToAtomType(element);
		this->atoms[i] = new Atom(type, pos);

	}
}

void Molecule::parseSDF_bonds(ifstream* sdf)
{
	string line;

	for (int i = 0; i < this->nBonds; i++)
	{

		getline(*sdf, line);
		int atom1 = stoi(line.substr(0, 3))-1;
		int atom2 = stoi(line.substr(3, 3))-1;
		string order = line.substr(6, 3);

		BondType type = strToBondType(order);
		this->bonds[i] = new Bond(type, *this->atoms[atom1], *this->atoms[atom2]);

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

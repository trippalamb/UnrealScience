#include "..\Headers\Chemistry.h"
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace Chemistry;

//#TODO: create trim front of string method
//#TODO: create a make sure first character is capitalized method
//#TODO: add a bunch of atom types
static unordered_map<string, AtomType> const str2type = { 
	{"Hydrogen", AtomType::Hydrogen}, 
	{"hydrogen", AtomType::Hydrogen}, 
	{" H", AtomType::Hydrogen}, 
	{"H", AtomType::Hydrogen}, 
	{"1", AtomType::Hydrogen}, 
	{"Oxygen", AtomType::Oxygen},
	{"oxygen", AtomType::Oxygen},
	{" O", AtomType::Oxygen},
	{"O", AtomType::Oxygen},
	{"8", AtomType::Oxygen}
};

//#TODO: find the way to properly build these maps for structs of data 
//       so that you get get various naming choices back out from AtomType
static unordered_map<AtomType, string> const type2str = {
	{AtomType::Hydrogen, "H"},
	{AtomType::Oxygen, "O"}
};

//#TODO: find a way to map to instances of atomic properties
static unordered_map<AtomType, double> const type2radius = {
	{AtomType::Hydrogen, 0.012},
	{AtomType::Oxygen, 0.0152}
};

//#TODO: generalize the repeated code inside of all of these functions
AtomType Chemistry::StrToAtomType(string str)
{
	auto it = str2type.find(str);
	if (it != str2type.end())
	{
		return it->second;
	}
	else 
	{ 
		cout << "This string is not associated with a supported AtomType.";
		exit(EXIT_FAILURE);
	}
}

string Chemistry::AtomTypeToStr(AtomType type)
{
	auto it = type2str.find(type);
	if (it != type2str.end())
	{
		return it->second;
	}
	else
	{
		cout << "This atom type does not have an associated string.";
		exit(EXIT_FAILURE);
	}
}

double Chemistry::GetAtomicRadius(AtomType type)
{
	auto it = type2radius.find(type);
	if (it != type2radius.end())
	{
		return it->second;
	}
	else
	{
		cout << "This atom type does not have an associated radius yet.";
		exit(EXIT_FAILURE);
	}
}

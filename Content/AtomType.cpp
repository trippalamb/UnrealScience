#include "Chemistry.h"
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace Chemistry;

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

static unordered_map<AtomType, string> const type2str = {
	{AtomType::Hydrogen, "H"},
	{AtomType::Oxygen, "O"}
};


AtomType Chemistry::strToAtomType(string str)
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

string Chemistry::atomTypeToStr(AtomType type)
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
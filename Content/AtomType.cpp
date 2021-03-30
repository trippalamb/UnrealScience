#include "..\Headers\Chemistry.h"
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace Chemistry;

string capitalize(string in);

//#TODO: add a bunch of atom types
static unordered_map<string, AtomType> const str2type = { 
	{"Hydrogen", AtomType::Hydrogen}, 
	{"H", AtomType::Hydrogen}, 
	{"1", AtomType::Hydrogen}, 
	{"Helium", AtomType::Helium}, 
	{"He", AtomType::Helium}, 
	{"2", AtomType::Helium}, 
	{"Lithium", AtomType::Lithium}, 
	{"Li", AtomType::Lithium}, 
	{"3", AtomType::Lithium}, 
	{"Beryllium", AtomType::Beryllium}, 
	{"Be", AtomType::Beryllium}, 
	{"4", AtomType::Beryllium}, 
	{"Boron", AtomType::Boron}, 
	{"B", AtomType::Boron}, 
	{"5", AtomType::Boron}, 
	{"Carbon", AtomType::Carbon}, 
	{"C", AtomType::Carbon}, 
	{"6", AtomType::Carbon}, 
	{"Nitrogen", AtomType::Nitrogen}, 
	{"N", AtomType::Nitrogen}, 
	{"7", AtomType::Nitrogen}, 
	{"Oxygen", AtomType::Oxygen},
	{"O", AtomType::Oxygen},
	{"8", AtomType::Oxygen},
	{"Flourine", AtomType::Flourine}, 
	{"F", AtomType::Flourine}, 
	{"9", AtomType::Flourine}, 
	{"Neon", AtomType::Neon}, 
	{"Ne", AtomType::Neon}, 
	{"10", AtomType::Neon}, 
	{"Sodium", AtomType::Sodium}, 
	{"Na", AtomType::Sodium}, 
	{"11", AtomType::Sodium}, 
	{"Magnesium", AtomType::Magnesium}, 
	{"Mg", AtomType::Magnesium}, 
	{"12", AtomType::Magnesium},
	{"Aluminum", AtomType::Aluminum}, 
	{"Al", AtomType::Aluminum}, 
	{"13", AtomType::Aluminum}, 
	{"Silicon", AtomType::Silicon}, 
	{"Si", AtomType::Silicon}, 
	{"14", AtomType::Silicon}, 
	{"Phosphorus", AtomType::Phosphorus}, 
	{"P", AtomType::Phosphorus}, 
	{"15", AtomType::Phosphorus}, 
	{"Sulfur", AtomType::Sulfur}, 
	{"S", AtomType::Sulfur}, 
	{"16", AtomType::Sulfur}, 
	{"Chlorine", AtomType::Chlorine}, 
	{"Ch", AtomType::Chlorine}, 
	{"17", AtomType::Chlorine}, 
	{"Argon", AtomType::Argon}, 
	{"Ar", AtomType::Argon}, 
	{"18", AtomType::Argon}
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


template <class T, class U>
T useMap(unordered_map<U, T> map, U in)
{
	auto it = map.find(in);
	if (it != map.end())
	{
		return it->second;
	}
	else
	{
		cout << "This mapping is not yet supported.";
		exit(EXIT_FAILURE);
	}
}

AtomType Chemistry::StrToAtomType(string str)
{
	return useMap<AtomType, string>(str2type, capitalize(str));
}

string Chemistry::AtomTypeToStr(AtomType type)
{
	return useMap<string, AtomType>(type2str, type);
}

double Chemistry::GetAtomicRadius(AtomType type)
{
	return useMap<double, AtomType>(type2radius, type);
}


string capitalize(string in)
{
	string result = in;
	result[0] = toupper(result[0]);
	return result;
}

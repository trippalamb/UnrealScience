#include "..\Headers\Chemistry.h"
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace Chemistry;

static unordered_map<string, BondType> const str2type = {
	{"Single", BondType::Single},
	{"single", BondType::Single},
	{"1", BondType::Single},
	{"  1", BondType::Single},
	{"Double", BondType::Double},
	{"double", BondType::Double},
	{"1", BondType::Double},
	{"  2", BondType::Double},
	{"Triple", BondType::Triple},
	{"triple", BondType::Triple},
	{"3", BondType::Triple},
	{"  3", BondType::Triple}
};

static unordered_map<BondType, string> const type2str = {
	{BondType::Single, "-"},
	{BondType::Double, "="},
	{BondType::Triple, "≡"}
};


BondType Chemistry::StrToBondType(string str)
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

string Chemistry::BondTypeToStr(BondType type)
{
	auto it = type2str.find(type);
	if (it != type2str.end())
	{
		return it->second;
	}
	else
	{
		cout << "This bond type does not have an associated string.";
		exit(EXIT_FAILURE);
	}
}


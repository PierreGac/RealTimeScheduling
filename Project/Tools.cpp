#include "Tools.h"


Tools::Tools()
{
}


Tools::~Tools()
{
}



std::vector<std::string> Tools::split(const std::string &s, char delim)
{
	vector<string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}
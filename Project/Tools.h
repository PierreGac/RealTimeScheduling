#pragma once
using namespace std;
#include <string>
#include <vector>
#include <sstream>

class Tools
{
public:
	static vector<string> split(const string &str, char delim);
	Tools();
	~Tools();
};


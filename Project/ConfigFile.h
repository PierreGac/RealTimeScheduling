#include <vector>
#include "PrioritySchedule.h"
using namespace std;


class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();
	void OpenFile(const char* path);
	void Start(void);

	vector<string> *Commands;
	
	Schedule* CurrentSchedule;

	unsigned int CommandLineInterpreter(const string &line);

private:
	unsigned int _index = 0;
};


#include "PrioritySchedule.h"
#include "DeadlineSchedule.h"
#include "QueueSchedule.h"
using namespace std;


class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	void OpenFile(const char* path, const int & size);
	void Start(void);
	long int GetLoops(void);
	bool SafeExit(void);
private:
	int _size;
	vector<string> *Commands;
	bool _isSafeExit;
	Schedule* CurrentSchedule;

	unsigned int CommandLineInterpreter(const string &line);

	unsigned int _index = 0;
	unsigned int _exitWait = 0;
};


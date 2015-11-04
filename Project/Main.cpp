#include <stdio.h>
#include <iostream>
#include <time.h>
//#include "MainThread.h"
#include "ConfigFile.h"
#include <wiringPi.h>
#include <sys/mman.h>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		mlockall(MCL_FUTURE | MCL_CURRENT);
		//pthread_t
		std::ios::sync_with_stdio(false);

		ConfigFile conf;

		if (argc == 2)
			conf.OpenFile(argv[1], 5000);
		else
			conf.OpenFile(argv[1], atoi(argv[2]));
		conf.Start();

		do
		{
			//_sleep(50);
			delay(50);
		} while (!conf.SafeExit());

		//_sleep(1000);
		delay(1000);
		cout << endl << endl << "-- END [" << conf.GetLoops() << "] loops --" << endl;
		getchar();
	}
	else
		cout << "No file" << endl;
}
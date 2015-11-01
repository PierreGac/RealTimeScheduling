#include <stdio.h>
#include <iostream>
#include <time.h>
#include "MainThread.h"
#include "ConfigFile.h"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		std::ios::sync_with_stdio(false);
		srand(time(NULL));

		ConfigFile conf;

		conf.OpenFile(argv[1]);
		conf.Start();

		_sleep(1000);
		cout << endl << endl << "-- END --" << endl;
		//getchar();
	}
}
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "MainThread.h"
#include "ConfigFile.h"
using namespace std;

int main()
{
	std::ios::sync_with_stdio(false);
	srand(time(NULL)); // initialisation de rand

	ConfigFile conf;
	
	conf.OpenFile("toto.rif");
	conf.Start();
	


	/*cout << "Number of rooms: " << flush;
	string str;
	getline(cin, str);
	int number = atoi(str.c_str());

	cout << "Start..." << endl;
	MainThread *mainThread = new MainThread(number);
	
	mainThread->StartUI();

	free(mainThread);*/

	cout << endl << endl << "-- END --" << endl;
	getchar();
}
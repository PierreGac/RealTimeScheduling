#include<stdio.h>
#include<iostream>
#include "MainThread.h"
using namespace std;

int main()
{
	std::ios::sync_with_stdio(false);
	srand(time(NULL)); // initialisation de rand


	cout << "Start...\r\n";
	MainThread *mainThread = new MainThread();
	
	mainThread->StartUI();

	free(mainThread);
}
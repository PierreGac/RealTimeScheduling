#ifndef INCLUDE_MAINTHREAD
#define INCLUDE_MAINTHREAD
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <strstream>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include "Room.h"
#include "Tools.h"
using namespace std;

class Room;

class MainThread
{
public:
	MainThread(const int& number);
	~MainThread();
	void MakeHeader_TOP(strstream &sstr) const;
	void MakeHeader_BOTTOM(strstream &sstr) const;
	string PrintSystemStatus() const;
	int GetRoomsCount() const;
	int StartRoomThread() const;
	void StartUI();
	bool CheckCommands(string &str);
	void SchedulePriority();
	void ScheduleDeadLine();
	static bool Sort(const Room* r1, const Room* r2);
	void ProcessTasks(Room* r);
	void StartMainThread(int type);
	void PrintExecutionTime(const std::chrono::duration<double> &time_span);
	void PriorityEvent(Room* room);

	//Priorities
	static const unsigned char __PRIORITY_LOW = 0;
	static const unsigned char __PRIORITY_NORMAL = 1;
	static const unsigned char __PRIORITY_HIGH = 2;
	static const unsigned char __PRIORITY_TOP = 3;

private:
	vector<Room*> Rooms;
	thread *t;
	int _roomSize;
	bool _isExit;
	bool _onPriorityEvent;
	bool _onMacro;
	bool _nextPrintTime;
	vector<Room*> _eventRooms;
	vector<Room*> _fixedArray;
};
#endif
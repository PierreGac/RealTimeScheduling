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
	void ScheduleBasic();
	void ScheduleDeadLine();
	static bool Sort(const Room* r1, const Room* r2);
	static bool SortDeadline(const Room* r1, const Room* r2);
	void ProcessTasks(Room* r);
	void StartMainThread(int type);
	void PrintExecutionTime(const std::chrono::duration<double> &time_span);
	void PriorityEvent(Room* room);

private:
	vector<Room*> Rooms;
	thread *t;
	int _roomSize;
	bool _isExit;
	bool _onPriorityEvent;
	bool _onMacro;
	bool _nextPrintTime;
	vector<Room*> _eventRooms;
	std::chrono::high_resolution_clock::time_point _t1;
	std::chrono::high_resolution_clock::time_point _t2;
	std::chrono::duration<double> _time_span1;
	vector<Room*> _fixedArray;
};
#endif
#ifndef INCLUDE_SCHEDULE
#define INCLUDE_SCHEDULE
#include "Room.h"
#include <vector>
#include <cstdint>
using namespace std;

class Room;

class Schedule
{
public:
	Schedule();
	virtual ~Schedule() = 0;

	virtual void StartSchedule(void) = 0;
	virtual void Init(unsigned int size) = 0;
	virtual void PriorityEvent(Room* room) = 0;
	virtual void ProcessTasks(Room* room) = 0;
	virtual void StartTimedThread(void) = 0;

	vector<Room*> Rooms;
	vector<Room*> _fixedArray;
	vector<Room*> _eventRooms;

	int RoomSize;
	int LoopWait;
	int ExecTime;
	uint64_t LoopsCounter;

	thread* scheduleThread;
	thread* execTimeThread;

	bool _onMacro;
	bool _isExit;
	bool _onPriorityEvent;
	bool _printTime;
	bool _isLoopWait;

	std::chrono::high_resolution_clock::time_point _t1;
	std::chrono::high_resolution_clock::time_point _t2;
	std::chrono::duration<double> _time_span1;




	//Priorities
	static const unsigned char __PRIORITY_LOW = 0;
	static const unsigned char __PRIORITY_NORMAL = 1;
	static const unsigned char __PRIORITY_HIGH = 2;
	static const unsigned char __PRIORITY_TOP = 3;

private:
	virtual void Thread(void) = 0;
	virtual void ScheduleTimedLifeThread() = 0;
};
#endif

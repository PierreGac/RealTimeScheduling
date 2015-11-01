#ifndef INCLUDE_SCHEDULE
#define INCLUDE_SCHEDULE
#include "Room.h"
#include <vector>
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

	vector<Room*> Rooms;
	vector<Room*> _fixedArray;
	int RoomSize;
	thread* scheduleThread;
private:
	virtual void Thread(void) = 0;
};
#endif

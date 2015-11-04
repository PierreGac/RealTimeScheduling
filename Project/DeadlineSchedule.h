#include "Schedule.h"
using namespace std;

class DeadlineSchedule : public Schedule
{
public:
	DeadlineSchedule();
	~DeadlineSchedule();

	void StartSchedule(void);
	void Init(unsigned int size);
	void PriorityEvent(Room* room);
	void ProcessTasks(Room* room);
	static bool Sort(const Room* r1, const Room* r2);
	void StartTimedThread(void);
private:
	void Thread(void);
	void ScheduleTimedLifeThread(void);
	RoomData* _roomData;
};

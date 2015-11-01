#include "Schedule.h"
class PrioritySchedule : public Schedule
{
public:
	PrioritySchedule();
	~PrioritySchedule();

	void StartSchedule(void);
	void Init(unsigned int size);
	void PriorityEvent(Room* room);
	void Thread(void);
};


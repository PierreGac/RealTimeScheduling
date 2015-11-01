#include "PrioritySchedule.h"


PrioritySchedule::PrioritySchedule()
{
}


PrioritySchedule::~PrioritySchedule()
{
	for (int i = 0; i < RoomSize; i++)
	{
		free(Rooms[i]);
	}
}


void PrioritySchedule::StartSchedule(void)
{
	
}

void PrioritySchedule::Thread(void)
{
	bool temp = false;
	do
	{
		if (!_onMacro)
		{
			_t1 = high_resolution_clock::now();
			//1
			sort(Rooms.begin(), Rooms.end(), Sort);

			//2
			for (int i = 0; i < _roomSize; i++)
			{
				//Check if we have a priority event
				if (_onPriorityEvent)
				{
					//LOCK THE VECTOR
					temp = true;
					//Sort the priority vector:
					sort(_eventRooms.begin(), _eventRooms.end(), Sort);
					//Process the priority vector
					vector< Room* >::iterator it = _eventRooms.begin();
					while (it != _eventRooms.end())
					{
						if ((*it)->GetEmmergencyState())
						{
							cout << endl << "The room [";
							cout << (*it)->GetID();
							cout << "] is in an emmergency state!" << endl;
						}
						ProcessTasks(*it);
						it = _eventRooms.erase(it);
					}
					_onPriorityEvent = false; //Semaphore ?
					//UNLOCK THE VECTOR
				}
				if (Rooms[i]->Priority == __PRIORITY_LOW)
					break;
				ProcessTasks(Rooms[i]);
			}

			_t2 = high_resolution_clock::now();
			_time_span1 = duration_cast<duration<double>>(_t2 - _t1);
			if (_time_span1.count() != 0)
				printf(":: Duration time: %f ::\r\n", _time_span1.count());
			/*if ((temp || _nextPrintTime) && _time_span1.count() != 0)
			{
			printf("%f -- %f :: Duration time: %f\r\n", _t2, _t1, _time_span1.count());
			temp = false;
			_nextPrintTime = false;
			}*/
			///_sleep(50);
		}
	} while (!_isExit);
}

void PrioritySchedule::Init(unsigned int size)
{
	RoomSize = size;
	Rooms = vector<Room*>(size);
	_fixedArray = vector<Room*>(size);
	//_eventRooms = vector<Room*>();
	for (int i = 0; i < size; i++)
	{
		Rooms[i] = new Room(i, this);
		_fixedArray[i] = Rooms[i];
	}
}


void PrioritySchedule::PriorityEvent(Room* room)
{

}
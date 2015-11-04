#include "DeadlineSchedule.h"
#include "UI.h"

DeadlineSchedule::DeadlineSchedule()
{
	_printTime = false;
	_onMacro = false;
	_onPriorityEvent = false;
	_isExit = false;
	_isLoopWait = false;
	_isRandom = false;
	_autoPrint = true;
	ExecTime = 0;
}

DeadlineSchedule::~DeadlineSchedule()
{
	for (int i = 0; i < RoomSize; i++)
	{
		free(Rooms[i]);
	}
	free(_roomData);
	free(scheduleThread);
}

bool DeadlineSchedule::Sort(const Room* r1, const Room* r2)
{
	return r1->Deadline > r2->Deadline;
}

void DeadlineSchedule::StartTimedThread(void)
{
	execTimeThread = new thread(&DeadlineSchedule::ScheduleTimedLifeThread, this);
}

void DeadlineSchedule::ScheduleTimedLifeThread(void)
{
	high_resolution_clock::time_point start = high_resolution_clock::now();
	high_resolution_clock::time_point t;
	double milli;
	ExecTime = ExecTime / 1000;
	do
	{
		t = high_resolution_clock::now();
		milli = duration_cast<duration<double>>(t - start).count();
	} while (milli < ExecTime);
	_isExit = true;
	UI::Print("> Max execution time reached... abort");
	stringstream sstr;
	sstr << "Amount of loops: ";
	sstr << LoopsCounter << endl;
	UI::Print(sstr.str());
}

void DeadlineSchedule::StartSchedule(void)
{
	scheduleThread = new thread(&DeadlineSchedule::Thread, this);
}

void DeadlineSchedule::Thread(void)
{
	bool temp = false;
	stringstream sstr;
	vector< Room* >::iterator it;
	LoopsCounter = 0;
	do
	{
		if (!_onMacro)
		{
			if (_isRandom)
			{
				for (int i = 0; i < RoomSize; i++)
				{
					Rooms[i]->RandomBehaviour();
				}
			}
			_t1 = high_resolution_clock::now();
			//Sort the array
			for (int i = 0; i < RoomSize; i++)
			{
				Rooms[i]->GetDeadlineValue();
			}

			sort(Rooms.begin(), Rooms.end(), Sort);

			for (int i = 0; i < RoomSize; i++)
			{
				//Check if we have a priority event
				if (_onPriorityEvent)
				{
					//LOCK THE VECTOR ?
					/*if (_autoPrint)
						temp = true;*/
					//Sort the priority vector:
					sort(_eventRooms.begin(), _eventRooms.end(), Sort);
					//Process the priority vector
					it = _eventRooms.begin();
					while (it != _eventRooms.end())
					{
						if ((*it)->GetEmmergencyState() && _autoPrint)
						{
							sstr.str(string());
							sstr << endl << "> The room [";
							sstr << (*it)->GetID();
							sstr << "] is in an emmergency state!" << endl;
							UI::Print(sstr.str());
						}
						ProcessTasks(*it);
						it = _eventRooms.erase(it);
					}
					_onPriorityEvent = false; //Semaphore ?
					//UNLOCK THE VECTOR
				}
				ProcessTasks(Rooms[i]);
			}

			_t2 = high_resolution_clock::now();
			_time_span1 = duration_cast<duration<double>>(_t2 - _t1);
			if ((temp || _printTime) && _time_span1.count() != 0)
			{
				sstr.str(string());
				sstr << "> Loop time: ";
				sstr << _time_span1.count() << endl;
				UI::Print(sstr.str());
				temp = false;
				_printTime = false;
			}

			LoopsCounter++;

			if (_isLoopWait)
			{
				//Loop wait here:
				///delay(50);
			}
		}
	} while (!_isExit);
}

void DeadlineSchedule::Init(unsigned int size)
{
	RoomSize = size;
	Rooms = vector<Room*>(size);
	_fixedArray = vector<Room*>(size);
	//_eventRooms = vector<Room*>();
	for (unsigned int i = 0; i < size; i++)
	{
		Rooms[i] = new Room(i, this);
		_fixedArray[i] = Rooms[i];
	}
}

void DeadlineSchedule::PriorityEvent(Room* room)
{
	_eventRooms.push_back(room);
	_onPriorityEvent = true;
}

void DeadlineSchedule::ProcessTasks(Room* room)
{
	_roomData = room->GetRoomData();
#pragma region Emmergency
	if (_roomData->Emmergency)
	{
		/*cout << "The room ";
		cout << r->GetID();
		cout << " is in an emmergency state \tTime: ";*/
		/*_t2 = high_resolution_clock::now();
		_time_span1 = duration_cast<duration<double>>(_t2 - _t1);
		cout << _time_span1.count() << endl;*/
		room->SetEmmergencyState(false);
	}

#pragma endregion
#pragma region Door
	//Request action on a door:
	if (_roomData->DoorSensor && _roomData->DoorState == 0)
	{
		//cout << "Open door" << endl;
		room->SetDoorState(true);
		if (_roomData->RoomPresence)
		{
			room->SetRoomPresence(false); //Leaving the room
		}
		else
			room->SetRoomPresence(true); //Enter the room
	}
	else
	{
		if (_roomData->DoorState == 1 && !_roomData->DoorSensor)
			room->SetDoorState(false);
	}
#pragma endregion

#pragma region Light
	//Request action on a door:
	if (_roomData->LightSensor && room->GetRoomPresence())
	{
		room->SetLightState(true);
	}
	else
	{
		room->SetLightState(false);
		if (!room->GetRoomPresence())
			room->SetLightSensorState(false);
	}
#pragma endregion
#pragma region HEATER
	room->UpdateTemperature();
	if (_roomData->Temperature < 18 && !_roomData->HeaterState)
		room->SetHeaterState(true);
	else
		if (_roomData->Temperature > 23 && _roomData->HeaterState)
			room->SetHeaterState(false);
#pragma endregion
}


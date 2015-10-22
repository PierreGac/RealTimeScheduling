#include "MainThread.h"

MainThread::MainThread()
{
	_isExit = false;
	_nextPrintTime = false;
	_onPriorityEvent = false;
	_roomSize = 100;
	Rooms = vector<Room*>(_roomSize);
	_fixedArray = vector<Room*>(_roomSize);
	_eventRooms = vector<Room*>();
	for (int i = 0; i < _roomSize; i++)
	{
		Rooms[i] = new Room(i, this);
		_fixedArray[i] = Rooms[i];
	}
}


MainThread::~MainThread()
{
	for (int i = 0; i < _roomSize; i++)
	{
		free(Rooms[i]);
	}
}

int MainThread::GetRoomsCount() const
{
	return Rooms.size();
}

string MainThread::PrintSystemStatus() const
{
	strstream sstr;
	sstr << "|-------------------------|" << endl;
	sstr << "|PRIORITY|ID |    DOOR    |" << endl;
	sstr << "|        |   |STATE|SENSOR|" << endl;
	for (int i = 0; i < _roomSize; i++)
	{
		sstr << Rooms.at(i)->ToString();
		sstr << "\r\n";
	}
	sstr << "|-------------------------|" << endl;
	sstr << '\0';
	return sstr.str();
}


int MainThread::StartRoomThread() const
{
	/*cout << "Starting room threads...";
	for (int i = 0; i < _roomSize; i++)
	{
	Rooms[i]->StartThread();
	}
	cout << "...done!" << endl;*/
	return 0;
}


void MainThread::StartUI()
{
	//Start UI:
	cout << "Ready" << endl;
	string str;
	bool state = false;
	steady_clock::time_point t1, t2;
	duration<double> time_span;
	do
	{
		cout << "\r\nCommand: " << flush;
		getline(cin, str);
		//Switch the string:
		t1 = steady_clock::now();
		CheckCommands(str);
		t2 = steady_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		_sleep(10);
		PrintExecutionTime(time_span);
	} while (str != "exit");
}

void MainThread::PrintExecutionTime(const duration<double> &time_span)
{
	if (time_span.count() == 0)
		return;
	cout << "|-- Command execution time: ";
	cout << time_span.count();
	cout << " --|" << endl;
}

void MainThread::PriorityEvent(Room* room)
{
	/*cout << "Priority event: ";
	cout << room->GetID() << endl;*/
	_eventRooms.push_back(room);
	_onPriorityEvent = true;
}

void MainThread::ScheduleDeadLine()
{
	//1
	//Create a vector for each priority
	vector<Room*> low = vector<Room*>();
	vector<Room*> medium = vector<Room*>();
	vector<Room*> high = vector<Room*>();
	vector<Room*> top = vector<Room*>();

	for (int i = 0; i < _roomSize; i++)
	{
		switch (Rooms[i]->Priority)
		{
		case __PRIORITY_LOW:
			low[low.size() - 1] = Rooms[i];
			break;
		case __PRIORITY_NORMAL:
			medium[medium.size() - 1] = Rooms[i];
			break;
		case __PRIORITY_HIGH:
			high[high.size() - 1] = Rooms[i];
			break;
		case __PRIORITY_TOP:
			top[top.size() - 1] = Rooms[i];
			break;
		}
	}
	//Reorganize the array:
	for (int i = 0; i < _roomSize; i++)
	{

	}
	//2


	//3
}

bool MainThread::Sort(const Room* r1, const Room *r2)
{
	return r1->Priority > r2->Priority;
}


void MainThread::SchedulePriority()
{
	steady_clock::time_point t1, t2;
	duration<double> time_span;
	bool temp = false;
	do
	{
		t1 = steady_clock::now();
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

		t2 = steady_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		if ((temp || _nextPrintTime) && time_span.count() != 0)
		{
			printf("%f -- %f :: Duration time: %f\r\n", t2, t1, time_span.count());
			temp = false;
			_nextPrintTime = false;
		}
	} while (!_isExit);
}

void MainThread::ProcessTasks(Room *r)
{
	RoomData data = r->GetRoomData();
#pragma region Emmergency
	if (data.Emmergency)
	{
		cout << "The room ";
		cout << r->GetID();
		cout << "is in an emmergency state" << endl;

		r->SetEmmergencyState(false);
	}

#pragma endregion
#pragma region Door
	//Request action on a door:
	if (data.DoorSensor && data.DoorState == 1)
	{
		//cout << "Open door" << endl;
		r->SetDoorState(true);
	}
	else
	{
		if (data.DoorState == 0 && !data.DoorSensor)
			r->SetDoorState(false);
	}
#pragma endregion
}


void MainThread::StartMainThread(int type)
{
	if (type == 1)
		t = new thread(&MainThread::SchedulePriority, this);
	else
		t = new thread(&MainThread::ScheduleDeadLine, this);
}













bool MainThread::CheckCommands(string &str)
{
	if (str.length() == 0)
		return false;
	std::vector<std::string> subStr = Tools::split(str, ' ');
	if (subStr[0] == "exit")
	{
		_isExit = true;
		return true;
	}
	if (subStr.at(0) == "time")
	{
		_nextPrintTime = true;
		return true;
	}
	if (subStr[0] == "start")
	{
		StartRoomThread();
		if (subStr.size() == 2)
		{
			StartMainThread(atoi(subStr.at(1).c_str()));
		}
		else
		{
			StartMainThread(1);
			return true;
		}
	}
	if (subStr[0] == "print")
	{
		if (subStr.size() == 2)
		{
			if (subStr[1] == "all")
			{
				cout << PrintSystemStatus() << endl;
				return true;
			}
			//Get the room ID:
			for (int i = 0; i < _roomSize; i++)
			{
				if (Rooms[i]->GetID() == atoi(subStr.at(1).c_str()))
				{
					cout << Rooms[i]->ToString() << endl;
					return true;
				}
			}
			return false;
		}
		if (subStr.size() == 3)
		{
			if (subStr[1] == "priority")
			{
				cout << "Printing rooms with priority: ";
				cout << subStr[2] << endl;
				int p = atoi(subStr.at(2).c_str());
				int counter = 0;
				for (int i = 0; i < _roomSize; i++)
				{
					if (Rooms[i]->Priority == p)
					{
						cout << "\t";
						cout << Rooms[i]->ToString() << endl;
						counter++;
					}
				}
				cout << "-- END [x";
				cout << counter;
				cout << "] --" << endl;
			}
		}
	}

	//Set commands
	if (subStr.at(0) == "set")
	{
		if (subStr.size() == 1)
		{
			cout << "set command, missing arguments" << endl;
			return false;
		}
		#pragma region DOOR
		if (subStr.at(1) == "door")
		{
			#pragma region SENSOR
			if (subStr.at(2) == "sensor")
			{
				if (subStr.at(3) == "all")
				{
					for (int i = 0; i < _roomSize; i++)
					{
						_nextPrintTime = true;
						if (atoi(subStr.at(4).c_str()) == 0)
							_fixedArray[i]->SetDoorSensorState(false);
						else
							_fixedArray[i]->SetDoorSensorState(true);
					}
					return true;
				}
				else
				{
					for (int i = 0; i < _roomSize; i++)
					{
						if (atoi(subStr.at(3).c_str()) == _fixedArray[i]->GetID())
						{
							_nextPrintTime = true;
							if (atoi(subStr.at(4).c_str()) == 0)
								_fixedArray[i]->SetDoorSensorState(false);
							else
								_fixedArray[i]->SetDoorSensorState(true);
							return true;
						}
					}
				}
			}
			#pragma endregion
			#pragma region STATE
			if (subStr.at(2) == "state")
			{
				if (subStr.at(3) == "all")
				{
					for (int i = 0; i < _roomSize; i++)
					{
						if (atoi(subStr.at(4).c_str()) == 0)
							_fixedArray[i]->SetDoorState(false);
						else
							_fixedArray[i]->SetDoorState(true);
						return true;
					}
				}
				else
				{
					for (int i = 0; i < _roomSize; i++)
					{
						if (atoi(subStr.at(3).c_str()) == _fixedArray[i]->GetID())
						{
							if (atoi(subStr.at(4).c_str()) == 0)
								_fixedArray[i]->SetDoorState(false);
							else
								_fixedArray[i]->SetDoorState(true);
							return true;
						}
					}
				}
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region Emmergency
		if (subStr.at(1) == "emmergency")
		{
			if (subStr.at(2) == "all")
			{
				bool state;
				if (atoi(subStr.at(3).c_str()) == 1)
					state = true;
				else
					state = false;
				for (int i = 0; i < _roomSize; i++)
				{
					_fixedArray[i]->SetEmmergencyState(state);
				}
				return true;
			}
			for (int i = 0; i < _roomSize; i++)
			{
				if (_fixedArray[i]->GetID() == atoi(subStr.at(2).c_str()))
				{
					if (atoi(subStr.at(3).c_str()) == 1)
						_fixedArray[i]->SetEmmergencyState(true);
					else
						_fixedArray[i]->SetEmmergencyState(true);
					return true;
				}
			}
		}
		#pragma endregion

		#pragma region PRESENCE
		if (subStr.at(1) == "presence")
		{
			if (subStr.size() != 4)
			{
				cout << "set presence : Missing parameters" << endl;
				return false;
			}
			if (subStr.at(2) == "all")
			{
				bool state;
				if (atoi(subStr.at(3).c_str()) == 1)
					state = true;
				else
					state = false;
				for (int i = 0; i < _roomSize; i++)
				{
					_fixedArray[i]->SetRoomPresence(state);
				}
				return true;
			}
			for (int i = 0; i < _roomSize; i++)
			{
				if (_fixedArray[i]->GetID() == atoi(subStr.at(2).c_str()))
				{
					if (atoi(subStr.at(3).c_str()) == 1)
						_fixedArray[i]->SetRoomPresence(true);
					else
						_fixedArray[i]->SetRoomPresence(true);
					return true;
				}
			}
		}
		#pragma endregion
	}

	//Get commands
	if (subStr.at(0) == "get")
	{

	}

	return false;
}
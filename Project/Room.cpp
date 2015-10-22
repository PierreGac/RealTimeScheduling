#include "Room.h"

Room::Room(const int &id, MainThread* mainThread)
{
	_mainThread = mainThread;
	ID = id;
	_temperature = 22;
	_heaterState = false;

	_doorSensor = false; //No door presence
	_doorState = 1; //Door closed
	Priority = __PRIORITY_NORMAL; //Normal priority
}

Room::~Room()
{

}

string Room::ToString() const
{
	strstream sstr;


	sstr << "[";
	sstr << Priority;
	sstr << "]  ";
	sstr << ID;
	sstr << "-- Door state: ";
	sstr << _doorState;
	sstr << "-- Door sensor: ";
	sstr << _doorSensor;
	sstr << '\0';

	return sstr.str();
}

void Room::RoomThread()
{
	threadState = __FLAG_THREAD_RUNNING;
	int i = 0;
	duration<double> time_span;
	steady_clock::time_point t1 = steady_clock::now();
	steady_clock::time_point t2 = steady_clock::now();
	cout.precision(5);
	do
	{
		//Do some room stuff here
		GetDoorSensor();
		//usleep #include <unistd.h>
	} while (threadState == __FLAG_THREAD_RUNNING);

	threadState = __FLAG_THREAD_STOPPED;
}

int Room::StartThread()
{
	t = new	thread(&Room::RoomThread, this);
	return 0;
}

void Room::StopThread()
{
	threadState = __FLAG_THREAD_STOP;
}

int Room::GetID() const
{
	return ID;
}

#pragma region Operator
Room & Room::operator=(const Room & r)
{
	if (this != &r)
	{
		/*_monomes.clear(); //On vide le vecteur
		for (int cnt = 0; cnt < p.GetTaille(); cnt++)
		{
			_monomes.push_back(p.GetMonome(cnt));
		}*/
	}
	return *this;
}

bool Room::operator<(const Room* r) const 
{ 
	return Priority < r->Priority;
}


std::ostream & operator<<(std::ostream & flot, const Room & R)
{
	flot << R.ToString();

	return flot;
}
#pragma endregion Operator


RoomData& Room::GetRoomData(void) const
{
	RoomData data = RoomData();
	data.DoorSensor = _doorSensor;
	data.DoorState = _doorState;

	return data;
}
#pragma region DOOR
int Room::GetDoorState(void) const
{
	return _doorState;
}

// true = Request Open
// false = Request Close
void Room::SetDoorState(const bool& state)
{
	_doorState = 2; //On progress
	//Sending closing order : Wait
	_sleep(50);
	//Priority = __PRIORITY_NORMAL;
	_doorActuatorThread = new thread(&Room::WaitClosingTime, this, state); //Simulate the opening command
}

void Room::WaitClosingTime(bool state)
{
	//cout << "toto" << endl;
	_sleep(10000);
	if (state)
		_doorState = 0; //Open
	else
		_doorState = 1; //Closed
	Priority = __PRIORITY_NORMAL;
}

bool Room::GetDoorSensor() const
{
	_sleep(10);
	return _doorSensor;
}

void Room::SetDoorSensorState(const bool &state)
{
	_sleep(10);
	if (state)
	{
		if (Priority < __PRIORITY_HIGH)
			Priority = __PRIORITY_HIGH;
	}
	_doorSensor = state;
}

#pragma endregion

#pragma region Emmergency
void Room::SetEmmergencyState(const bool &state)
{
	_emmergencyState = state;
	if (state)
	{
		Priority = __PRIORITY_TOP;
		_mainThread->PriorityEvent(this);
	}
	else
	{
		if (Priority == __PRIORITY_TOP)
			Priority = __PRIORITY_NORMAL;
	}
}

bool Room::GetEmmergencyState(void) const
{
	return _emmergencyState;
}
#pragma endregion
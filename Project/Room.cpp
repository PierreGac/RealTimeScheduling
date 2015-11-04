#include "Room.h"

Room::Room(const int &id, Schedule* schedule)
{
	_schedule = schedule;
	_rndTick = rand() % 1000;
	ID = id;
	_temperature = 22;
	_heaterState = false;
	_data = new RoomData();

	_doorSensor = false; //No door presence
	_doorState = 0; //Door closed
	_emmergencyState = false;
	_lightSensor = false;
	_lightState = false;
	_roomPresence = false;
	Priority = Schedule::__PRIORITY_LOW; //Default is low
}

Room::~Room()
{
	free(_data);
}

string Room::ToString() const
{
	stringstream sstr;


	sstr << "|    ";
	sstr << (int)Priority;
	sstr << "   |";
	sstr << ID;
	if (ID < 10)
	sstr << "  | ";
	else
	{
		if (ID < 100)
			sstr << " | ";
		else
			sstr << "| ";
	}
	sstr << _emmergencyState;
	sstr << " | ";
	sstr << _roomPresence;
	sstr << " |  ";
	sstr << (int)_doorState;
	sstr << "  |  ";
	sstr << _doorSensor;
	sstr << "   |  ";
	sstr << _lightState;
	sstr << "  |  ";
	sstr << _lightSensor;
	sstr << "   |";
	sstr << _temperature;
	sstr << "|  ";
	sstr << _heaterState;
	sstr << "  |";
	sstr << '\0';

	return sstr.str();
}

void Room::GetDeadlineValue(void)
{
	if ((unsigned char)_doorSensor != _doorState)
		Deadline++;
	if (_emmergencyState)
		Deadline++;
	if (_lightSensor != _lightState)
		Deadline++;
	if (_roomPresence)
		Deadline++;
	if (_heaterState && _temperature > 23)
		Deadline++;
	if (!_heaterState && _temperature < 18)
		Deadline++;
}

void Room::RoomThread(void)
{
	threadState = __FLAG_THREAD_RUNNING;
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


RoomData* &Room::GetRoomData(void)
{
	_data->DoorSensor = _doorSensor;
	_data->DoorState = _doorState;
	_data->Emmergency = _emmergencyState;
	_data->RoomPresence = _roomPresence;
	_data->LightSensor = _lightSensor;
	_data->LightState = _lightState;
	_data->Temperature = _temperature;
	_data->HeaterState = _heaterState;
	return _data;
}
#pragma region DOOR
unsigned char Room::GetDoorState(void) const
{
	return _doorState;
}

// true = Request Open
// false = Request Close
void Room::SetDoorState(const bool& state)
{
	_doorState = 2; //On progress
	//Sending closing order : Wait
	//_sleep(50);
	//Priority = __PRIORITY_NORMAL;
	WaitClosingTime(state);
	
}

void Room::WaitClosingTime(bool state)
{
	if (state)
		_doorState = 1; //Open
	else
		_doorState = 0; //Closed
	Priority = Schedule::__PRIORITY_NORMAL;
}

bool Room::GetDoorSensor() const
{
	//_sleep(10);
	return _doorSensor;
}

void Room::SetDoorSensorState(const bool &state)
{
	if (Priority < Schedule::__PRIORITY_HIGH)
		Priority = Schedule::__PRIORITY_HIGH;
	_doorSensor = state;
}

#pragma endregion

#pragma region Emmergency
void Room::SetEmmergencyState(const bool &state)
{
	_emmergencyState = state;
	if (state)
	{
		_emmergencyTriggerTime = high_resolution_clock::now();
		Priority = Schedule::__PRIORITY_TOP;
		_schedule->PriorityEvent(this);
	}
	else
	{
		if (Priority == Schedule::__PRIORITY_TOP && _roomPresence)
			Priority = Schedule::__PRIORITY_NORMAL;
		else
			if (!_roomPresence)
				Priority = Schedule::__PRIORITY_LOW;
	}
}

bool Room::GetEmmergencyState(void) const
{
	return _emmergencyState;
}
#pragma endregion


#pragma region PRESENCE
void Room::SetRoomPresence(const bool &state)
{
	_roomPresence = state;
	if (_roomPresence)
	{
		if (Priority == Schedule::__PRIORITY_LOW)
			Priority = Schedule::__PRIORITY_NORMAL;
	}
	else
	{
		Priority = Schedule::__PRIORITY_LOW;
	}
}

bool Room::GetRoomPresence(void) const
{
	return _roomPresence;
}

#pragma endregion

#pragma region LIGHT
bool Room::GetLightState(void) const
{
	return _lightState;
}

// true = Request Open
// false = Request Close
void Room::SetLightState(const bool& state)
{
	_lightState = state;
	if (Priority == Schedule::__PRIORITY_HIGH)
		Priority = Schedule::__PRIORITY_NORMAL; //Set the priority back to normal
}

bool Room::GetLightSensor() const
{
	//_sleep(10);
	return _lightSensor;
}

void Room::SetLightSensorState(const bool &state)
{
	//_sleep(10);
	if (state)
	{
		if (Priority < Schedule::__PRIORITY_HIGH)
			Priority = Schedule::__PRIORITY_HIGH;
	}
	_lightSensor = state;
}

#pragma endregion

#pragma region HEATER
void Room::UpdateTemperature(void)
{
	//If heating
	if (_heaterState)
	{
		if (_temperature < 25.0)
			_temperature += 0.0000001;
	}
	else
	{
		if (_temperature > 15.0)
			_temperature -= 0.0000001;

	}
}

bool Room::GetHeaterState(void) const
{
	return _heaterState;
}

// true = Request Open
// false = Request Close
void Room::SetHeaterState(const bool& state)
{
	_heaterState = state;
}

double Room::GetTemperature() const
{
	//_sleep(10);
	return _temperature;
}

#pragma endregion



void Room::RandomBehaviour(void)
{
	if (_rndTick > TICK_VAL)
	{
		int rnd = rand() % 100;
		_rndTick = 0;
		if (rnd > 50)
			return;
		//If the room is not empty:
		if (_roomPresence)
		{
			if (rand() % 100 == 1)
				SetEmmergencyState(true);
			if (rand() % 100 == 1)
				SetDoorSensorState(!_doorSensor);
			if (rand() % 100 == 1)
				SetLightSensorState(!_lightSensor);
		}
		else
		{
			//EMMERGENCY-DOOR
			if (rand() % 100 == 1)
				SetEmmergencyState(true);
			if (rand() % 100 == 1)
				SetDoorSensorState(!_doorSensor);
		}
		return;
	}
	_rndTick++;
}
#ifndef INCLUDE_ROOM
#define INCLUDE_ROOM
#include <string>
#include <stdio.h>
#include<strstream>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include "MainThread.h"
#include "Schedule.h"
using namespace std;
using namespace std::chrono;

class Schedule;

struct RoomData
{
	bool DoorSensor;
	int DoorState;
	bool Emmergency;
	bool RoomPresence;
	bool LightSensor;
	bool LightState;
	double Temperature;
	bool HeaterState;
};
class Room
{
public:
	Room(const int & ID, Schedule* mainThread);
	~Room();
	void Init();
	string ToString() const;
	unsigned char Priority;
	unsigned char Deadline;
	void GetDeadlineValue(void);
	int GetID() const;
#pragma region Operator
	Room & operator=(const Room & p);
	inline bool operator>(const Room & r) { return Priority > r.Priority; }
	bool operator<(const Room* r) const;
	inline bool operator>=(const Room & r) { return !(Priority < r.Priority); }
	inline bool operator<=(const Room & r) { return !(Priority > r.Priority); }
#pragma endregion Operator

#pragma region DOOR
	void SetDoorState(const bool &state);			//Set the state of the door (0:closed, 1:open)
	void SetDoorSensorState(const bool &state);		//Set the state of the door sensor (0:empty, 1:someone)
	bool GetDoorSensor(void)const;					//Get the state of the door sensor
	unsigned char GetDoorState(void)const;					//Get the state of the door (2:opening/closing)
#pragma endregion
#pragma region LIGHT
	void SetLightState(const bool &state);			//Set the state of the light (0:off, 1:on)
	void SetLightSensorState(const bool &state);	//Set the state of the light sensor (0:empty, 1:someone)
	bool GetLightSensor(void)const;					//Get the state of the light sensor
	bool GetLightState(void)const;					//Get the state of the light
#pragma endregion
#pragma region EMMERGENCY
	void SetEmmergencyState(const bool &state);		//Set the emmergency state (0:off, 1:on) =>triggers the top priority
	bool GetEmmergencyState(void) const;			//Get the emmergency state
#pragma endregion
#pragma region PRESENCE
	void SetRoomPresence(const bool &state);		//Set the room presence (0:empty, 1:notEmpty)
	bool GetRoomPresence(void) const;
#pragma endregion
#pragma region HEATER
	void SetHeaterState(const bool &state);			//Set the state of the heater (0:off, 1:on)
	bool GetHeaterState(void) const;				//Get the state of the heater
	double GetTemperature(void) const;				//Get the temperature of the room
	void UpdateTemperature(void);					//As we don't have a real system, we have to update the temperature manually
#pragma endregion

	RoomData* &GetRoomData();					//Get a structure of the room data

#pragma region CONST
	static const unsigned char __FLAG_THREAD_STOPPED = 0;
	static const unsigned char __FLAG_THREAD_RUNNING = 1;
	static const unsigned char __FLAG_THREAD_STOP = 2;
	static const int __VALUE_WAIT = 200; // 200ms
	static const int __VALUE_GET_INPUT_TIME = 10;

	//DOOR
	static const unsigned char __DOOR_CLOSED = 0;
	static const unsigned char __DOOR_OPEN = 1;
	static const unsigned char __DOOR_ACTION = 2;

#pragma endregion
private:
	Schedule* _mainThread;
	void RoomThread();
	int ID;
	unsigned char _doorState;									// 0:open, 1:closed, 2:action in progress
	bool _doorSensor;								// 0:empty, 1:someone
	bool _emmergencyState;							// 0:none, 1:emmergency
	bool _lightState;
	bool _lightSensor;
	double _temperature;
	bool _heaterState;
	bool _roomPresence;								//Indicates if the room is used or not
	unsigned char HeaterState;
	unsigned char threadState;
	RoomData* _data = NULL;

	void WaitClosingTime(bool state);
	thread* _doorActuatorThread = NULL;

};

#pragma region Operator
std::ostream & operator<<(std::ostream & flot, const Room & R);
#pragma endregion Operator


#endif
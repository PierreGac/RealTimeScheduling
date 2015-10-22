#ifndef INCLUDE_ROOM
#define INCLUDE_ROOM
#include <string>
#include<strstream>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include "MainThread.h"
using namespace std;
using namespace std::chrono;

class MainThread;

struct RoomData
{
	bool DoorSensor;
	int DoorState;
};
class Room
{
public:
	Room(const int & ID, MainThread* mainThread);
	~Room();
	void Init();
	string ToString() const;
	int StartThread();
	void StopThread();
	int Priority;
	int GetID() const;
#pragma region Operator
	Room & operator=(const Room & p);
	inline bool operator>(const Room & r) { return Priority > r.Priority; }
	bool operator<(const Room* r) const;
	inline bool operator>=(const Room & r) { return !(Priority < r.Priority); }
	inline bool operator<=(const Room & r) { return !(Priority > r.Priority); }
#pragma endregion Operator

#pragma region DOOR
	void SetDoorState(const bool &state);			//Set the state of the door (0:open, 1:closed)
	void SetDoorSensorState(const bool &state);		//Set the state of the door sensor (0:empty, 1:someone)
	bool GetDoorSensor(void)const;					//Get the state of the door sensor
	int GetDoorState(void)const;					//Get the state of the door (2:opening/closing)
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
	RoomData& GetRoomData() const;					//Get a structure of the room data
private:
	MainThread* _mainThread;
	void RoomThread();
	int ID;

	int _doorState;									// 0:open, 1:closed, 2:action in progress
	bool _doorSensor;								// 0:empty, 1:someone
	bool _emmergencyState;							// 0:none, 1:emmergency
	bool _lightState;
	double _temperature;
	bool _heaterState;
	int HeaterState;
	thread *t;
	int threadState;

	void WaitClosingTime(bool state);
	thread* _doorActuatorThread;


	static const int __FLAG_THREAD_STOPPED = 0;
	static const int __FLAG_THREAD_RUNNING = 1;
	static const int __FLAG_THREAD_STOP = 2;
	static const int __VALUE_WAIT = 200; // 200ms
	static const int __VALUE_GET_INPUT_TIME = 10;


	//Prioorities
	static const int __PRIORITY_LOW = 0;
	static const int __PRIORITY_NORMAL = 1;
	static const int __PRIORITY_HIGH = 2;
	static const int __PRIORITY_TOP = 3;
};

#pragma region Operator
std::ostream & operator<<(std::ostream & flot, const Room & R);
#pragma endregion Operator


#endif
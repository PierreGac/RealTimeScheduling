#include "ConfigFile.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Tools.h"
#include "UI.h"

ConfigFile::ConfigFile()
{
	Commands = NULL;
}


ConfigFile::~ConfigFile()
{
	free(CurrentSchedule);
}


void ConfigFile::OpenFile(const char* path)
{
	if (Commands != NULL)
		free(Commands);
	Commands = new vector<string>();
	ifstream file;
	file.open(path);
	if (file.is_open())
	{
		UI::Print("> Reading the file...");
		for (string line; getline(file, line);)
		{
			//Process each line:
			if (line[0] == '#') //If we have a command instruction
			{
				//Push the line without the #
				Commands->push_back(line.substr(1, line.size() - 1));
			}
		}
	}
	else
		UI::Print("> Unable to open the file");
	file.close();
}

void ConfigFile::Start(void)
{
	UI::Print("> Starting process...");
	for (int i = 0; i < Commands->size(); i++)
	{
		CommandLineInterpreter(Commands->at(i));
	}
}

unsigned int ConfigFile::CommandLineInterpreter(const string &line)
{
	if (line.length() == 0)
		return 0;
	int tempINT = 0;
	std::vector<std::string> subStr = Tools::split(line, ' ');

#pragma region Set schedule type
	if (subStr[0] == "schedule")
	{
		//Check if the line is correct:
		if (subStr.size() < 2)
			return 0;
		//Switch the type
		if (subStr[1] == "priority")
		{
			CurrentSchedule = new PrioritySchedule();
			if (subStr.size() == 2)
				CurrentSchedule->Init(5000);
			else
			{
				//Get the size
				CurrentSchedule->Init(atoi(subStr[2].c_str()));
			}
			return 1;
		}
	}
#pragma endregion

#pragma region PRINT
	if (subStr[0] == "print")
	{
		if (subStr.size() == 2)
		{
			if (subStr[1] == "all")
			{
				UI::PrintSystemStatus(CurrentSchedule->Rooms, CurrentSchedule->RoomSize);
				return 1;
			}
			return 1;
		}
		if (subStr.size() == 3)
		{
			if (subStr[1] == "priority")
			{
				int p = atoi(subStr[2].c_str());
				stringstream sstr;
				sstr << "Printing rooms with priority: ";
				sstr << p << '\0';
				UI::Print(sstr.str());

				UI::PrintMakeHeader_TOP();
				int counter = 0;
				for (int i = 0; i < CurrentSchedule->RoomSize; i++)
				{
					if ((int)CurrentSchedule->Rooms[i]->Priority == p)
					{
						UI::PrintRoomRaw(CurrentSchedule->Rooms[i]->ToString());
						counter++;
					}
				}
				UI::PrintMakeHeader_BOTTOM();
				sstr.str(string());
				sstr << "-- END [x";
				sstr << counter;
				sstr << "] --" << endl;
				UI::Print(sstr.str());
				return 1;
			}
			if (subStr[1] == "room")
			{
				//Get the room ID:
				tempINT = atoi(subStr[2].c_str());
				for (int i = 0; i < CurrentSchedule->RoomSize; i++)
				{
					if (CurrentSchedule->Rooms[i]->GetID() == tempINT)
					{
						UI::PrintRoom(CurrentSchedule->Rooms[i]->ToString());
						return 1;
					}
				}
				return 0;
			}
		}
		if (subStr.size() > 1)
		{
			stringstream sstr;
			sstr << "> ";
			for (int i = 1; i < subStr.size(); i++)
			{
				sstr << subStr[i];
				sstr << " ";
			}
			sstr << '\0';
			UI::Print(sstr.str());
		}
		else
			UI::PrintSystemStatus(CurrentSchedule->Rooms, CurrentSchedule->RoomSize);
		return 1;
	}
#pragma endregion
#pragma region Start schedule
	if (subStr[0] == "start")
	{
		UI::Print("> Starting schedule...");
		CurrentSchedule->StartSchedule();
		UI::Print("> Schedule started");
		return 1;
	}
	if (subStr[0] == "exit")
	{
		CurrentSchedule->_isExit = true;
		stringstream sstr;
		sstr << "Amount of loops: ";
		sstr << CurrentSchedule->LoopsCounter << endl;
		UI::Print(sstr.str());
	}
#pragma endregion
#pragma region Time
	if (subStr[0] == "time")
	{
		CurrentSchedule->_printTime = true;
		return 1;
	}
	if (subStr[0] == "freeze")
	{
		CurrentSchedule->_onMacro = true;
		return 1;
	}
	if (subStr[0] == "unfreeze")
	{
		CurrentSchedule->_onMacro = false;
		return 1;
	}
	if (subStr[0] == "loopwait")
	{
		if (subStr.size() == 2)
		{
			CurrentSchedule->_isLoopWait = true;
			CurrentSchedule->LoopWait = atoi(subStr[1].c_str());
			return 1;
		}
		return 0;
	}
	if (subStr[0] == "exectime")
	{
		if (subStr.size() == 2)
		{
			CurrentSchedule->ExecTime = atoi(subStr[1].c_str());
			return 1;
		}
		return 0;
	}
	if (subStr[0] == "timethread")
	{
		CurrentSchedule->StartTimedThread();
		return 1;
	}
	if (subStr[0] == "wait")
	{
		if (subStr.size() == 2)
		{
			stringstream sstr;
			sstr << "> Waiting ";
			sstr << subStr[1];
			sstr << " ms" << '\0';
			UI::Print(sstr.str());
			_sleep(atoi(subStr[1].c_str()));
			//delay(atoi(substr[1].c_str()));
			return 1;
		}
		UI::Print("> Waiting 500ms");
		_sleep(500);
		//delay(500);
		return 1;
	}
#pragma endregion

#pragma region SET
	//Set commands
	if (subStr[0] == "set")
	{
		if (subStr.size() == 1)
		{
			UI::Print("Set command, missing arguments");
			return 0;
		}
#pragma region DOOR
		if (subStr[1] == "door")
		{
#pragma region SENSOR
			if (subStr[2] == "sensor")
			{
				if (subStr[3] == "all")
				{
					CurrentSchedule->_printTime = true;
					tempINT = atoi(subStr[4].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == 0)
							CurrentSchedule->_fixedArray[i]->SetDoorSensorState(false);
						else
							CurrentSchedule->_fixedArray[i]->SetDoorSensorState(true);
					}
					return 1;
				}
				else
				{
					tempINT = atoi(subStr[3].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == CurrentSchedule->_fixedArray[i]->GetID())
						{
							CurrentSchedule->_printTime = true;
							if (atoi(subStr[4].c_str()) == 0)
								CurrentSchedule->_fixedArray[i]->SetDoorSensorState(false);
							else
								CurrentSchedule->_fixedArray[i]->SetDoorSensorState(true);
							return 1;
						}
					}
					return 0;
				}
			}
#pragma endregion
#pragma region STATE
			if (subStr[2] == "state")
			{
				if (subStr[3] == "all")
				{
					CurrentSchedule->_printTime = true;
					tempINT = atoi(subStr[4].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == 0)
							CurrentSchedule->_fixedArray[i]->SetDoorState(false);
						else
							CurrentSchedule->_fixedArray[i]->SetDoorState(true);
					}
					return 1;
				}
				else
				{
					tempINT = atoi(subStr[3].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == CurrentSchedule->_fixedArray[i]->GetID())
						{
							if (atoi(subStr[4].c_str()) == 0)
								CurrentSchedule->_fixedArray[i]->SetDoorState(false);
							else
								CurrentSchedule->_fixedArray[i]->SetDoorState(true);
							return 1;
						}
					}
					return 0;
				}
			}
#pragma endregion
		}
#pragma endregion

#pragma region Emmergency
		if (subStr[1] == "emmergency")
		{
			if (subStr[2] == "all")
			{
				bool state;
				if (atoi(subStr[3].c_str()) == 1)
					state = true;
				else
					state = false;
				for (int i = 0; i < CurrentSchedule->RoomSize; i++)
				{
					CurrentSchedule->_fixedArray[i]->SetEmmergencyState(state);
				}
				return true;
			}
			tempINT = atoi(subStr[2].c_str());
			for (int i = 0; i < CurrentSchedule->RoomSize; i++)
			{
				if (CurrentSchedule->_fixedArray[i]->GetID() == tempINT)
				{
					if (atoi(subStr[3].c_str()) == 1)
						CurrentSchedule->_fixedArray[i]->SetEmmergencyState(true);
					else
						CurrentSchedule->_fixedArray[i]->SetEmmergencyState(true);
					return true;
				}
			}
		}
#pragma endregion

#pragma region PRESENCE
		if (subStr[1] == "presence")
		{
			if (subStr.size() != 4)
			{
				UI::Print("> Set presence : Missing parameters");
				return 0;
			}
			if (subStr[2] == "all")
			{
				bool state;
				if (atoi(subStr[3].c_str()) == 1)
					state = true;
				else
					state = false;
				for (int i = 0; i < CurrentSchedule->RoomSize; i++)
				{
					CurrentSchedule->_fixedArray[i]->SetRoomPresence(state);
				}
				return 1;
			}
			tempINT = atoi(subStr[2].c_str());
			for (int i = 0; i < CurrentSchedule->RoomSize; i++)
			{
				if (CurrentSchedule->_fixedArray[i]->GetID() == tempINT)
				{
					if (atoi(subStr[3].c_str()) == 1)
						CurrentSchedule->_fixedArray[i]->SetRoomPresence(true);
					else
						CurrentSchedule->_fixedArray[i]->SetRoomPresence(false);
					return true;
				}
			}
		}
#pragma endregion

#pragma region LIGHT
		if (subStr[1] == "light")
		{
#pragma region SENSOR
			if (subStr[2] == "sensor")
			{
				if (subStr[3] == "all")
				{
					CurrentSchedule->_printTime = true;
					tempINT = atoi(subStr[4].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == 0)
							CurrentSchedule->_fixedArray[i]->SetLightSensorState(false);
						else
							CurrentSchedule->_fixedArray[i]->SetLightSensorState(true);
					}
					return 1;
				}
				else
				{
					tempINT = atoi(subStr[3].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == CurrentSchedule->_fixedArray[i]->GetID())
						{
							CurrentSchedule->_printTime = true;
							if (atoi(subStr[4].c_str()) == 0)
								CurrentSchedule->_fixedArray[i]->SetLightSensorState(false);
							else
								CurrentSchedule->_fixedArray[i]->SetLightSensorState(true);
							return 1;
						}
					}
					return 0;
				}
			}
#pragma endregion
#pragma region STATE
			if (subStr[2] == "state")
			{
				if (subStr[3] == "all")
				{
					tempINT = atoi(subStr[4].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == 0)
							CurrentSchedule->_fixedArray[i]->SetLightState(false);
						else
							CurrentSchedule->_fixedArray[i]->SetLightState(true);
						return 1;
					}
				}
				else
				{
					tempINT = atoi(subStr[3].c_str());
					for (int i = 0; i < CurrentSchedule->RoomSize; i++)
					{
						if (tempINT == CurrentSchedule->_fixedArray[i]->GetID())
						{
							if (atoi(subStr[4].c_str()) == 0)
								CurrentSchedule->_fixedArray[i]->SetLightState(false);
							else
								CurrentSchedule->_fixedArray[i]->SetLightState(true);
							return 1;
						}
					}
					return 0;
				}
			}
#pragma endregion
		}
#pragma endregion
	}
#pragma endregion
#pragma region GET
	//Get commands
	if (subStr[0] == "get")
	{

	}
#pragma endregion


	return 0;
}

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
		for (string line; getline(file, line);)
		{
			//Process each line:
			if (line[0] == '#') //If we have a command instruction
			{
				//Push the line without the #
				//line.erase(remove(line.begin(), line.end(), '\n'), line.end());
				cout << line.size() << endl;
				Commands->push_back(line.substr(1, line.size() - 1));
			}
		}
	}
	file.close();
}

void ConfigFile::Start(void)
{
	for (int i = 0; i < Commands->size(); i++)
	{
		CommandLineInterpreter(Commands->at(i));
	}
}

unsigned int ConfigFile::CommandLineInterpreter(const string &line)
{
	if (line.length() == 0)
		return 0;
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
				CurrentSchedule->Init(100);
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
				int p = atoi(subStr.at(2).c_str());
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
				for (int i = 0; i < CurrentSchedule->RoomSize; i++)
				{
					if (CurrentSchedule->Rooms[i]->GetID() == atoi(subStr.at(2).c_str()))
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

#pragma endregion
#pragma region MISC
	if (subStr[0] == "wait")
	{
		if (subStr.size() == 2)
		{
			stringstream sstr;
			sstr << "> Waiting ";
			sstr << subStr[1];
			sstr << " ms" << '\0';
			UI::Print(sstr.str());
			//delay(atoi(substr[1].c_str()));
			return 1;
		}
		stringstream sstr;
		sstr << "> Waiting 500 ms" << '\0';
		UI::Print(sstr.str());
		//delay(atoi(substr[1].c_str()));
		return 1;
	}
#pragma endregion
	return 0;
}

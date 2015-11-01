#include "UI.h"


UI::UI()
{
}


UI::~UI()
{
}

void UI::Print(const string & str)
{
	cout << str << endl;
}

void UI::PrintSystemStatus(const vector<Room*> & rooms, const unsigned int & size)
{
	strstream sstr;
	MakeHeader_TOP(sstr);
	for (int i = 0; i < size; i++)
	{
		sstr << rooms.at(i)->ToString();
		sstr << endl;
	}
	MakeHeader_BOTTOM(sstr);
	
	//Print the string:
	cout << sstr.str() << endl;
}

void UI::PrintRoom(const string & roomSTR)
{
	strstream sstr;
	MakeHeader_TOP(sstr);
	sstr << roomSTR << endl;
	MakeHeader_BOTTOM(sstr);
	//Print the string:
	cout << sstr.str() << endl;
}

void UI::PrintRoomRaw(const string & roomSTR)
{
	//Print the string:
	cout << roomSTR << endl;
}

void UI::PrintMakeHeader_TOP(void)
{
	strstream sstr;
	MakeHeader_TOP(sstr);
	sstr << '\0';
	//Print the string:
	cout << sstr.str() << endl;
}

void UI::PrintMakeHeader_BOTTOM(void)
{
	strstream sstr;
	MakeHeader_BOTTOM(sstr);
	//Print the string:
	cout << sstr.str() << endl;
}

void UI::MakeHeader_TOP(strstream & sstr)
{
	sstr << "|---------------------------------------------------------|" << endl;
	sstr << "|       GLOBAL       |    DOOR    |    LIGHT   |  HEATER  |" << endl;
	sstr << "|PRIORITY|ID |URG|PRE|STATE|SENSOR|STATE|SENSOR|TEMP|STATE|" << endl;
}

void UI::MakeHeader_BOTTOM(strstream & sstr)
{
	sstr << "|---------------------------------------------------------|" << endl;
	sstr << '\0';
}
#include <vector>
#include "Room.h"
using namespace std;

class UI
{
public:
	UI();
	~UI();

	static void PrintSystemStatus(const vector<Room*> & rooms, const unsigned int & size);
	static void PrintRoom(const string & roomSTR);
	static void PrintRoomRaw(const string & roomSTR);
	static void Print(const string & str);
	static void PrintMakeHeader_TOP(void);
	static void PrintMakeHeader_BOTTOM(void);
	static void MakeHeader_TOP(strstream & sstr);
	static void MakeHeader_BOTTOM(strstream & sstr);
};


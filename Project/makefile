CC = g++
CFLAGS=-std=c++11 -lwiringPi -lpthread -Wno-deprecated

all: prog

prog: main.o UI.o Room.o Tools.o Schedule.o PrioritySchedule.o DeadlineSchedule.o QueueSchedule.o ConfigFile.o
	$(CC) $(CFLAGS) -o prog main.o ConfigFile.o UI.o Room.o Tools.o Schedule.o PrioritySchedule.o DeadlineSchedule.o QueueSchedule.o

main.o: main.cpp ConfigFile.h
	$(CC) $(CFLAGS) main.cpp

UI.o: UI.cpp UI.h
	$(CC) $(CFLAGS) UI.cpp

Room.o: Room.cpp Room.h
	$(CC) $(CFLAGS) Room.cpp

Tools.o: Tools.cpp Tools.h
	$(CC) $(CFLAGS) Tools.cpp

Schedule.o: Schedule.cpp Schedule.h
	$(CC) $(CFLAGS) Schedule.cpp

PrioritySchedule.o: PrioritySchedule.cpp PrioritySchedule.h
	$(CC) $(CFLAGS) PrioritySchedule.cpp

DeadlineSchedule.o: DeadlineSchedule.cpp DeadlineSchedule.h
	$(CC) $(CFLAGS) DeadlineSchedule.cpp

QueueSchedule.o: QueueSchedule.cpp QueueSchedule.h
	$(CC) $(CFLAGS) QueueSchedule.cpp

ConfigFile.o: ConfigFile.cpp ConfigFile.h
	$(CC) $(CFLAGS) ConfigFile.cpp

clean:
	del prog *.o

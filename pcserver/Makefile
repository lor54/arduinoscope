OBJS	= main.o serial.o
SOURCE	= main.c serial.c
HEADER	= serial.h
OUT	= pcserver
FLAGS	= -g -c -Wall
LFLAGS	= 
CC	= gcc

all:	pcserver

pcserver: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

serial.o: serial.c
	$(CC) $(FLAGS) serial.c 

clean:
	rm -f $(OBJS) $(OUT)
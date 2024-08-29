OBJS	= main.o serial.o utils.o
SOURCE	= main.c serial.c utils.c
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

utils.o: utils.c
	$(CC) $(FLAGS) utils.c 

clean:
	rm -f $(OBJS) $(OUT)
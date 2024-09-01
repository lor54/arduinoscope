SOURCE	= $(wildcard *.c) $(wildcard libs/*.c)
OBJS	= $(SOURCE:.c=.o)
HEADER	= $(wildcard *.h) $(wildcard libs/*.h)
OUT	= pcserver
FLAGS	= -g -c -Wall
LFLAGS	= 
CC	= gcc

all:	$(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(OUT)
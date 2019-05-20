CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm
OBJS = functions.o
TARGET = main
all: $(TARGET)

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o tp main.c $(CLIBS)
functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c $(CLIBS)
clean:
	rm -rf $(OBJS) tp

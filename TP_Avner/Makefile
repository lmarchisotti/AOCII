CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm
OBJS = cache.o
TARGET = main
all: $(TARGET)

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o tp2 main.c $(CLIBS)
cache.o: cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c $(CLIBS)
clean:
	rm -rf $(OBJS) tp2

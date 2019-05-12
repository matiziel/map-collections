CC=g++
CFLAGS=-Wall -std=c++11

all: main.cpp	TreeMap.h	HashMap.h
	$(CC)	main.cpp	$(CFLAGS)	-o	run

clean:
	rm -f *.o
	rm -f run

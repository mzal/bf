CXX = gcc
CFLAGS = -g -Wall
OBJS = main.o

all: main.o
	$(CXX) $(CFLAGS) -o bf $(OBJS)

main.o: main.c bf.c
	$(CXX) $(CFLAGS) -c -o main.o main.c

bf.o: bf.c
	$(CXX) $(CFLAGS) -c -o bf.o bf.c

CC = clang++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: main clean

main: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) $<

clean:
	rm *.o


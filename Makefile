CC = gcc
CFLAGS = -Wall -O2
LIBS = -lncurses

all: mytop

mytop: main.o monitor.o tui.o
	$(CC) $(CFLAGS) -o mytop main.o monitor.o tui.o $(LIBS)

main.o: main.c monitor.h tui.h
	$(CC) $(CFLAGS) -c main.c

monitor.o: monitor.c monitor.h
	$(CC) $(CFLAGS) -c monitor.c

tui.o: tui.c tui.h monitor.h
	$(CC) $(CFLAGS) -c tui.c

clean:
	rm -f *.o mytop


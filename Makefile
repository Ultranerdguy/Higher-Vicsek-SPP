CFLAGS = -Wall -lm -g
CC = cc

all: main.o particle.o
	$(CC) $(CFLAGS) main.o particle.o -o main
	make clean

main.o: main.c particle.h
	$(CC) $(CFLAGS) -c main.c

particle.o: particle.c particle.h
	$(CC) $(CFLAGS) -c particle.c

clean:
	rm particle.o main.o

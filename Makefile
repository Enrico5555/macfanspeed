CC=gcc

fanspeed : fanspeed.o
	$(CC) fanspeed.o -o fanspeed
fanspeed.o : fanspeed.c
	$(CC) -c fanspeed.c -o fanspeed.o

clean:
	rm *.o
	rm fanspeed

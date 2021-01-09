CC=gcc
OPT=-Wall

all: clean barber client

barber: barber.c lib.c queue.c
	$(CC) $^ -o $@ $(OPT)
client: client.c lib.c queue.c
	$(CC) $^ -o $@ $(OPT)

clean:
	rm -f client barber

CC=gcc
OPT=-Wall

all: clean barber client
clean:
	rm -f client barber
client: client.c utils.c queue_utils.c
	$(CC) $^ -o $@ $(OPT)
barber: barber.c utils.c queue_utils.c
	$(CC) $^ -o $@ $(OPT)

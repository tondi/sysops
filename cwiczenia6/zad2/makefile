CC=gcc
OPT=-Wall
LIBS= -lpthread

all: clean client barber

barber: barber.c lib.c queue.c
	$(CC) $^ -o $@ $(OPT) $(LIBS)
client: client.c lib.c queue.c
	$(CC) $^ -o $@ $(OPT) $(LIBS)

clean:
	rm -f client barber test

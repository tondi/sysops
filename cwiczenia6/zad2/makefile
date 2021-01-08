CC=gcc
OPT=-Wall
LIBS= -lpthread -lrt

all: clean c b

b: golibroda.c lib.c queue.c
	$(CC) $^ -o $@ $(OPT) $(LIBS)
c: client.c lib.c queue.c
	$(CC) $^ -o $@ $(OPT) $(LIBS)

clean:
	rm -f c b test

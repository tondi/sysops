CC=gcc
CFLAGS=-Wall -std=c11 -ggdb
CPPFLAGS=
SLAVE=client
MASTER=server
HEADER=message.h
FILE=input.txt

all: $(SLAVE) $(MASTER)

$(SLAVE): $(SLAVE).c $(HEADER)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(MASTER): $(MASTER).c $(HEADER)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

compile: 
	gcc -Wall -std=c11 -ggdb -o server server.c
	gcc -Wall -std=c11 -ggdb -o client client.c

.PHONY: clean

clean:
	-rm -rf *.o $(SLAVE) $(MASTER) 

test: $(MASTER) $(SLAVE)
	./$(MASTER) &
	./$(SLAVE) &
	./$(SLAVE) $(FILE) &
	

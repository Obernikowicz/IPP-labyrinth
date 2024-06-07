CC=gcc
CFLAGS=-Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2

all: labyrinth

labyrinth: input.o memory.o error.o dynamic_array.o validation.o labyrinth_data.o bitset.o converters.o bfs.o queue.o labyrinth.o
error.o: error.h
memory.o: memory.h error.h
input.o: input.h dynamic_array.h labyrinth_data.h error.h validation.h bitset.h converters.h
dynamic_array.o: dynamic_array.h memory.h
validation.o: validation.h dynamic_array.h labyrinth_data.h error.h
labyrinth_data.o: labyrinth_data.h dynamic_array.h memory.h bitset.h
bitset.o: bitset.h memory.h
converters.o: converters.h bitset.h
bfs.o: bfs.h labyrinth_data.h queue.h bitset.h dynamic_array.h
queue.o: queue.h memory.h
labyrinth.o: input.h dynamic_array.h error.h labyrinth_data.h validation.h bfs.h

clean:
	-rm *.o 
	-rm labyrinth
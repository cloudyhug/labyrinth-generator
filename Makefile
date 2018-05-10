CC=gcc
SOURCES=src/lgen.c src/bmpgen.c
OBJECTS=bin/lgen.o bin/bmpgen.o

all: createbin $(OBJECTS) bmplabyrinth

bin/lgen.o: src/lgen.c
	$(CC) -c $< -o $@

bin/bmpgen.o: src/bmpgen.c
	$(CC) -c $< -o $@

bmplabyrinth: src/main.c
	$(CC) $(OBJECTS) $< -o bin/$@

createbin:
	rm -rf bin
	mkdir bin

clean:
	rm -rf bin
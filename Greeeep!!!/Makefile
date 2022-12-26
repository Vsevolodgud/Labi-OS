TARGET = main
OBJECTS = main.o GrepHeader.o

CC = gcc

.PHONY: clean

$(TARGET): $(OBJECTS) ./GrepHeader.h
	$(CC) $(OBJECTS) -o $@

main.o: main.c
	$(CC) -c main.c
	
GrepHeader.o: GrepHeader.c
	$(CC) -c GrepHeader.c

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

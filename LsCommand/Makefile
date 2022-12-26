TARGET = main
OBJECTS = main.o
SOURCES = main.c

CC = gcc

.PHONY: run clean

$(TARGET): $(OBJECTS)
	$(CC) $< -o $@

$(OBJECTS): $(SOURCES)
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

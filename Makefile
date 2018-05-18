TARGET = wav-svg
LIBS = -lm
CC = g++
CFLAGS = -std=gnu++11 -Wall -W -Wextra -fexceptions -mthreads

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = Converter.o main.o AudioFile.o plugixml.o
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

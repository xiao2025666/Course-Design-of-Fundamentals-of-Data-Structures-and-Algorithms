CC = g++
CFLAGS = -Wall -O2 -std=c++17 -Iinclude
SOURCES = $(filter-out src/data_generator.c,$(wildcard src/*.c))
TARGET = 选课系统.exe

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

generator:
	$(CC) $(CFLAGS) src/data_generator.c -o data_generator.exe

clean:
	del /Q $(TARGET) data_generator.exe 2>NUL

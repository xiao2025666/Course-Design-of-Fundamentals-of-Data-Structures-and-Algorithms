CC = g++
CFLAGS = -Wall -O2 -std=c++17 -Iinclude -finput-charset=UTF-8 -fexec-charset=GBK
SOURCES = $(filter-out src/data_generator.c,$(wildcard src/*.c))
TARGET = 选课系统.exe

all:
	@chcp 936 >nul
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

generator:
	@chcp 936 >nul
	$(CC) $(CFLAGS) src/data_generator.c -o data_generator.exe

clean:
	@chcp 936 >nul
	del /Q $(TARGET) data_generator.exe 2>NUL
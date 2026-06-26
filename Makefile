CC = g++
CFLAGS = -Wall -O2 -std=c++17 -Iinclude -finput-charset=UTF-8 -fexec-charset=GBK
SOURCES = $(filter-out src/data_generator.c,$(wildcard src/*.c))
TARGET = 选课系统.exe

all:
	@chcp 65001 >nul
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

generator:
	@chcp 65001 >nul
	$(CC) $(CFLAGS) src/data_generator.c -o data_generator.exe

clean:
	@chcp 65001 >nul
	del /Q $(TARGET) data_generator.exe 2>NUL 
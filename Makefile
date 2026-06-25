CC = g++
CFLAGS = -Wall -O2 -std=c++17
SOURCES = main.c record_system.c cli_interface.c data_structure.c linked_list.c avl_tree.c hash_table.c storage_adapter.c file_io.c query_processor.c statistics.c batch_delete.c performance_test.c
TARGET = 选课系统.exe

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>NUL

# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g

# the build target executable:
DIR = bin
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).o tasks.o merge_sort.o
	$(CC) $(CFLAGS) -o ./$(DIR)/$(TARGET) ./$(DIR)/$(TARGET).o ./$(DIR)/tasks.o ./$(DIR)/merge_sort.o

$(TARGET).o : $(TARGET).c tasks.h merge_sort.h
	$(CC) $(CFLAGS) -o ./$(DIR)/$(TARGET).o -c $(TARGET).c

tasks.o : tasks.c tasks.h
	$(CC) $(CFLAGS) -o ./$(DIR)/tasks.o -c tasks.c

merge_sort.o : merge_sort.c merge_sort.h
	$(CC) $(CFLAGS) -o ./$(DIR)/merge_sort.o -c merge_sort.c


clean:
	$(RM) $(TARGET)
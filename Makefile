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

$(TARGET): $(TARGET).o tasks.o
	$(CC) $(CFLAGS) -o ./$(DIR)/$(TARGET) ./$(DIR)/$(TARGET).o ./$(DIR)/tasks.o

$(TARGET).o : $(TARGET).c tasks.h
	$(CC) $(CFLAGS) -o ./$(DIR)/$(TARGET).o -c $(TARGET).c

tasks.o : tasks.c tasks.h
	$(CC) $(CFLAGS) -o ./$(DIR)/tasks.o -c tasks.c

clean:
	$(RM) $(TARGET)
# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -g
# Executable name
TARGET = Trello.exe
# Source and object files
SRCS = main.c priority.c project.c task.c ui.c utils.c
OBJS = $(SRCS:.c=.o)
# Default target
all: $(TARGET)
# Linking object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
# Compiling each .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
# Clean only object files
clean:
	del /Q *.o 2>nul || true
# Optional: run the program
run: $(TARGET)
	$(TARGET)

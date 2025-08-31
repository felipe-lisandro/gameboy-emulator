CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

SRC = src/main.c src/cpu.c src/memory.c src/instructions.c
OBJ = src/main.o src/cpu.o src/memory.o src/instructions.o
TARGET = emulator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

src/cpu.o: src/cpu.c
	$(CC) $(CFLAGS) -c $< -o $@

src/memory.o: src/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

src/instructions.o: src/instructions.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

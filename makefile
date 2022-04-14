S_DIR=src
B_DIR=bin

CC=gcc
CFLAGS=-Wall -g
IN=$(S_DIR)/main.c
OUT= -o $(B_DIR)/output
SDL=-Ilibs/include -Llibs/lib -lmingw32 -lSDL2main -lSDL2

default:
	$(CC) $(IN) $(OUT) $(CFLAGS) $(SDL)
	./$(B_DIR)/output

clean:
	del /s /q $(B_DIR)\*
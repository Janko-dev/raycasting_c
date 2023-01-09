CC=gcc
CFLAGS=-Wall -g -Wextra -std=c11
IN= rays.c main.c
OUT= rays
SDL=-IC:\Users\janba\Dev\i686-w64-mingw32\include -LC:\Users\janba\Dev\i686-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 

default:
	$(CC) $(IN) -o $(OUT) $(CFLAGS) $(SDL)
CC=gcc
CFLAGS=-Wall -g -Wextra
IN= main.c
OUT= -o out
SDL=-IC:\Users\janba\Dev\i686-w64-mingw32\include -LC:\Users\janba\Dev\i686-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 

default:
	$(CC) $(IN) $(OUT) $(CFLAGS) $(SDL)
	./out
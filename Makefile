CFLAGS = -Iinc -Wall

main: src/*.c
	gcc $(CFLAGS) -o bin/test src/*.c

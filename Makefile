CFLAGS = -D_POSIX_C_SOURCE=200809L -Iinc -Wall -Wextra

main: src/*.c
	gcc $(CFLAGS) -o bin/test src/*.c

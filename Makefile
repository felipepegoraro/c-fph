CC=gcc
CFLAGS=-Wall -Werror -Wextra -lm
XFLAGS=-ledit -lssl -lcrypto
SRC=main.c ./src/custom_functions.c ./src/fph.c ./src/hash_table.c ./src/polish_notation.c

all:
	$(CC) $(CFLAGS) $(SRC) $(XFLAGS) -o main

clean:
	rm main

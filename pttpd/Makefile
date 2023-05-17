CC = gcc
CFLAGS = -std=c89 -Wall -Werror
LDFLAGS = -lssl -lcrypto

TEST_DIR = tests

tests:${TEST_DIR}/%.c
	$(CC) $(CFLAGS) $(LIBFLAGS)


CC = gcc
CFLAGS = -Wall -Wextra -O2
EXECUTABLES = hello uptime

all: $(EXECUTABLES)

hello: hello.c
	$(CC) $(CFLAGS) -o hello hello.c

uptime: uptime.c
	$(CC) $(CFLAGS) -o uptime uptime.c

clean:
	rm -f $(EXECUTABLES)

.PHONY: all hello uptime clean

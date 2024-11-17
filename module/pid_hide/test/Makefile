CC = gcc
CFLAGS = -Wall -O3 -fstack-protector-strong -fPIE -D_FORTIFY_SOURCE=3 -fstack-clash-protection -Ilib
LDFLAGS = -pie -z noexecstack -Wl,-z,relro,-z,now
TARGET = test
SRC = test.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC)
	strip $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

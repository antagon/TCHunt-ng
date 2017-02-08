CFLAGS = -std=gnu99 -O2 -pedantic -Wall -ggdb

TARGET = tchuntng
INSTALL_DIR = /usr/local/bin

.PHONY: all install uninstall clean

all: $(TARGET)

install:
	install --mode 755 $(TARGET) $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -f *.o $(TARGET)

$(TARGET): main.o test_magic.o test_entropy.o
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lmagic -lm

main.o: main.c
	$(CC) $(CFLAGS) -c $<

test_magic.o: test_magic.c
	$(CC) $(CFLAGS) -c $<

test_entropy.o: test_entropy.c
	$(CC) $(CFLAGS) -c $<


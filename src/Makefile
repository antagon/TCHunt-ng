CFLAGS = -O3 -std=gnu99 -pedantic -Wall -ggdb

TARGET = tchuntng
INSTALL_DIR = /usr/local/bin

.PHONY: all install uninstall clean

all: $(TARGET)

install:
	install --mode 755 $(TARGET) $(INSTALL_DIR)

uninstall:
	$(RM) $(INSTALL_DIR)/$(TARGET)

clean:
	$(RM) *.o $(TARGET)

$(TARGET): main.o test_magic.o test_entropy.o test.o
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lmagic -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


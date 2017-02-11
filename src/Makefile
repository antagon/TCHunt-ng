CFLAGS = -std=gnu99 -O2 -pedantic -Wall -ggdb

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

$(TARGET): main.o test_magic.o test_entropy.o
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lmagic -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


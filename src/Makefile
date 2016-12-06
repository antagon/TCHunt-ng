CFLAGS = -std=gnu99 -O2 -pedantic -Wall -ggdb

TARGET = tchuntng
INSTALL_DIR = /usr/local/sbin

.PHONY: clean install

all: $(TARGET)

install:
	install --mode 755 $(TARGET) $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

clean:
	rm -f *.o $(TARGET)

$(TARGET): main.o strolldir.o file_list.o test_magic.o test_chidist.o
	$(CC) $(CFLAGS) $^ -o $(TARGET) -lmagic -lm

main.o: main.c
	$(CC) $(CFLAGS) -c $<

strolldir.o: strolldir.c
	$(CC) $(CFLAGS) -c $<

file_list.o: file_list.c
	$(CC) $(CFLAGS) -c $<

test_magic.o: test_magic.c
	$(CC) $(CFLAGS) -c $<

test_chidist.o: test_chidist.c
	$(CC) $(CFLAGS) -c $<


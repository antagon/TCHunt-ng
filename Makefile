.PHONY: all clean install uninstall test

all:
	$(MAKE) -C src/

install:
	$(MAKE) -C src/ $@

uninstall:
	$(MAKE) -C src/ $@

clean:
	$(MAKE) -C src/ $@
	$(MAKE) -C test/ $@

test:
	$(MAKE) -C test/ $@


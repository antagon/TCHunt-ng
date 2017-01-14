.PHONY: clean install uninstall

all:
	$(MAKE) -C src/

install:
	$(MAKE) -C src/ $@

uninstall:
	$(MAKE) -C src/ $@

clean:
	$(MAKE) -C src/ $@


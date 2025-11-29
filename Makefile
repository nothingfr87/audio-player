GCC_ARGUMENTS = -Wall -O2 -Isrc/SDL/include -Isrc/SDL/src -lSDL2 -o 

build:
	gcc src/main.c $(GCC_ARGUMENTS) atulo

install: build
	mv atulo /usr/local/bin/atulo
	chmod 777 /usr/local/bin/atulo

uninstall:
	rm -f /usr/local/bin/atulo

clean:
	rm -f atulo 

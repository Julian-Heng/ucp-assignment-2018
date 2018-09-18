CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic

all: makedirs
	$(CC) $(CFLAGS) -c effects.c -o ./objects/effects.o

makedirs:
	bash -c 'if [[ ! -e ./build ]]; then mkdir -p build; fi'
	bash -c 'if [[ ! -e ./objects ]]; then mkdir -p objects; fi'

clean:
	rm -rfv ./build
	rm -rfv ./objects

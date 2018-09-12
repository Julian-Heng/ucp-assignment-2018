CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic

makedirs:
	bash -c 'if [[ ! -e ./build ]]; then mkdir -p build; fi'
	bash -c 'if [[ ! -e ./objects ]]; then mkdir -p objects; fi'

clean:
	rm -rfv ./build
	rm -rfv ./objects

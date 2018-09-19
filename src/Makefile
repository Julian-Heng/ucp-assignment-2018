CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic

all: makedirs
	$(CC) $(CFLAGS) -c effects.c -o ./objects/effects.o

docs:
	bash -c '{ cd ../docs || exit; } && pdflatex AssignmentDoc.tex'

makedirs:
	bash -c 'if [[ ! -e ./build ]]; then mkdir -p build; fi'
	bash -c 'if [[ ! -e ./objects ]]; then mkdir -p objects; fi'

clean:
	rm -rfv ./build
	rm -rfv ./objects
	rm -rfv ../docs/AssignmentDoc.pdf

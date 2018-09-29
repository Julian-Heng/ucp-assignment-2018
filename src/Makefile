CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic
DCFLAGS = $(CFLAGS) -g
DATE = $(shell date)
MOD_DATE = $(shell date -r ./turtle.c)
USER = $(shell whoami)
HOST = $(shell hostname)

OBJ = ./objects/tools.o \
	  ./objects/fileIO.o \
	  ./objects/effects.o
PROG = ./build/turtle

all: makedirs
	$(CC) $(CFLAGS) -c tools.c -o ./objects/tools.o
	$(CC) $(CFLAGS) -c fileIO.c -o ./objects/fileIO.o
	$(CC) $(CFLAGS) -c effects.c -o ./objects/effects.o
	$(CC) $(CFLAGS) -c turtle.c -o ./objects/turtle.o \
					-DCOMPILE_DATE="\"$(DATE)\"" \
					-DMOD_DATE="\"$(MOD_DATE)\"" \
					-DUSER="\"$(USER)\"" \
					-DHOST="\"$(HOST)\""
	$(CC) $(OBJ) ./objects/turtle.o -lm -o $(PROG)

simple: makedirs
	$(CC) $(CFLAGS) -c tools.c -o ./objects/tools.o
	$(CC) $(CFLAGS) -c fileIO.c -o ./objects/fileIO.o
	$(CC) $(CFLAGS) -c effects.c -o ./objects/effects.o
	$(CC) $(CFLAGS) -c turtle.c -o ./objects/turtle.o \
					-DSIMPLE=1 \
					-DCOMPILE_DATE="\"$(DATE)\"" \
					-DMOD_DATE="\"$(MOD_DATE)\"" \
					-DUSER="\"$(USER)\"" \
					-DHOST="\"$(HOST)\""
	$(CC) $(OBJ) ./objects/turtle.o -lm -o $(PROG)

debug: makedirs
	$(CC) $(DCFLAGS) -c tools.c -o ./objects/tools.o
	$(CC) $(DCFLAGS) -c fileIO.c -o ./objects/fileIO.o
	$(CC) $(DCFLAGS) -c effects.c -o ./objects/effects.o
	$(CC) $(DCFLAGS) -c turtle.c -o ./objects/turtle.o \
					 -DCOMPILE_DATE="\"$(DATE)\"" \
					 -DMOD_DATE="\"$(MOD_DATE)\"" \
					 -DUSER="\"$(USER)\"" \
					 -DHOST="\"$(HOST)\""
	$(CC) $(OBJ) ./objects/turtle.o -lm -g -o $(PROG)

test: makedirs debug
	$(CC) $(DCFLAGS) -c test.c -o ./objects/test.o
	$(CC) $(OBJ) ./objects/test.o -o ./build/test
	./build/test
	valgrind ./build/test

docs:
	@bash -c '{ cd ../docs || exit; } && pdflatex AssignmentDoc.tex'

makedirs:
	@if [ ! -e ./build ]; then \
		mkdir -p ./build; \
	fi
	@if [ ! -e ./objects ]; then \
		mkdir -p ./objects; \
	fi

clean:
	@rm -rfv ./build ./objects ../docs/AssignmentDoc.pdf

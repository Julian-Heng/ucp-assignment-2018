CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic
DCFLAGS = $(CFLAGS) -g
COMPILER_INFO = $(shell $(CC) --version | head -n 1)
ifneq ($(shell uname), Linux)
DATE = $(shell date "+%Y-%m-%dT%H:%M:%S%z")
MOD_DATE = $(shell stat -f %Sm -t "%Y-%m-%dT%H:%M:%S%z" ./turtle.c)
else
DATE = $(shell date --iso-8601=seconds)
MOD_DATE = $(shell date --iso-8601-seconds -r ./turtle.c)
endif
USER = $(shell whoami)
HOST = $(shell hostname)

OBJ = ./objects/tools.o \
	  ./objects/linkedList.o \
	  ./objects/fileIO.o \
	  ./objects/effects.o
PROG = ./build/turtle

all: makedirs info
	@echo -----[TurtleGraphics]-----
	$(CC) $(CFLAGS) -c tools.c -o ./objects/tools.o
	$(CC) $(CFLAGS) -c linkedList.c -o ./objects/linkedList.o
	$(CC) $(CFLAGS) -c fileIO.c -o ./objects/fileIO.o
	$(CC) $(CFLAGS) -c effects.c -o ./objects/effects.o
	$(CC) $(CFLAGS) -c turtle.c -o ./objects/turtle.o \
					-DCOMPILER_INFO="\"$(COMPILER_INFO)\"" \
					-DCOMPILE_DATE="\"$(DATE)\"" \
					-DMOD_DATE="\"$(MOD_DATE)\"" \
					-DUSER="\"$(USER)\"" \
					-DHOST="\"$(HOST)\""
	$(CC) $(OBJ) ./objects/turtle.o -lm -o $(PROG)

simple: makedirs info
	@echo -----[TurtleGraphics Simple]-----
	$(CC) $(CFLAGS) -c tools.c -o ./objects/tools.o
	$(CC) $(CFLAGS) -c linkedList.c -o ./objects/linkedList.o
	$(CC) $(CFLAGS) -c fileIO.c -o ./objects/fileIO.o
	$(CC) $(CFLAGS) -c effects.c -o ./objects/effects.o
	$(CC) $(CFLAGS) -c turtle.c -o ./objects/turtle.o \
					-DSIMPLE=1 \
					-DCOMPILER_INFO="\"$(COMPILER_INFO)\"" \
					-DCOMPILE_DATE="\"$(DATE)\"" \
					-DMOD_DATE="\"$(MOD_DATE)\"" \
					-DUSER="\"$(USER)\"" \
					-DHOST="\"$(HOST)\""
	$(CC) $(OBJ) ./objects/turtle.o -lm -o $(PROG)
	@echo

debug: makedirs info
	@echo -----[Debug]-----
	$(CC) $(DCFLAGS) -c tools.c -o ./objects/tools.o
	$(CC) $(DCFLAGS) -c linkedList.c -o ./objects/linkedList.o
	$(CC) $(DCFLAGS) -c fileIO.c -o ./objects/fileIO.o
	$(CC) $(DCFLAGS) -c effects.c -o ./objects/effects.o
	$(CC) $(DCFLAGS) -c turtle.c -o ./objects/turtle.o \
					 -DDEBUG=1 \
					 -DCOMPILER_INFO="\"$(COMPILER_INFO)\"" \
					 -DCOMPILE_DATE="\"$(DATE)\"" \
					 -DMOD_DATE="\"$(MOD_DATE)\"" \
					 -DUSER="\"$(USER)\"" \
					 -DHOST="\"$(HOST)\""
	$(CC) $(OBJ) ./objects/turtle.o -lm -g -o $(PROG)
	@echo

test: makedirs info debug
	@echo -----[Test]-----
	$(CC) $(DCFLAGS) -c test.c -o ./objects/test.o
	$(CC) $(OBJ) ./objects/test.o -o ./build/test
	./build/test
	if command -v valgrind; then \
		valgrind ./build/test; \
	fi
	@echo

docs:
	@bash -c '{ cd ../docs || exit; } && pdflatex AssignmentDoc.tex'

info:
	@echo -----[Makefile Variables]-----
	@echo "CC            = $(CC)"
	@echo "CFLAGS        = $(CFLAGS)"
	@echo "DCFLAGS       = $(DCFLAGS)"
	@echo "COMPILER_INFO = $(COMPILER_INFO)"
	@echo "DATE          = $(DATE)"
	@echo "MOD_DATE      = $(MOD_DATE)"
	@echo "USER          = $(USER)"
	@echo "HOST          = $(HOST)"
	@echo "OBJ           = $(OBJ)"
	@echo "PROG          = $(PROG)"
	@echo

makedirs:
	@if [ ! -e ./build ]; then \
		mkdir -p ./build; \
	fi
	@if [ ! -e ./objects ]; then \
		mkdir -p ./objects; \
	fi

clean:
	@rm -rfv ./build ./objects ../docs/AssignmentDoc.pdf

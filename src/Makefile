# Standard compiler and flags declaration
CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic
DCFLAGS = $(CFLAGS) -g

# Getting compiler version
COMPILER_INFO = \"$(shell $(CC) --version | head -n 1)\"

# Getting compiler date and last modified date
ifneq ($(shell uname), Linux)
    DATE = \"$(shell date "+%Y-%m-%dT%H:%M:%S%z")\"
    MOD_DATE = \"$(shell stat -f %Sm -t "%Y-%m-%dT%H:%M:%S%z" ./turtle.c)\"
else
    DATE = \"$(shell date --iso-8601=seconds)\"
    MOD_DATE = \"$(shell date --iso-8601=seconds -r ./turtle.c)\"
endif

# Getting user information for version message
USER = \"$(shell whoami)\"
HOST = \"$(shell hostname)\"

# Declare defines when compiling turtle
DEFAULT_DEFINES = -DCOMPILER_INFO="$(COMPILER_INFO)" \
					-DCOMPILE_DATE="$(DATE)" \
					-DMOD_DATE="$(MOD_DATE)" \
					-DUSER="$(USER)" \
					-DHOST="$(HOST)"

# Declare all objects for all targets
# SOURCE doesn't include turtle.c because of linking
# issues with test.c

OBJ = 	./objects/effects.o \
		./objects/fileIO.o \
		./objects/linkedList.o \
		./objects/tools.o \
		./objects/turtle.o

SIMPLE_OBJ = 	./simple/effects.o \
				./simple/fileIO.o \
				./simple/linkedList.o \
				./simple/tools.o \
				./simple/turtle.o

DEBUG_OBJ = ./debug/effects.o \
			./debug/fileIO.o \
			./debug/linkedList.o \
			./debug/tools.o \
			./debug/turtle.o

TEST_OBJ = 	./debug/effects.o \
			./debug/fileIO.o \
			./debug/linkedList.o \
			./debug/tools.o \
			./debug/test.o

EXEC = turtle

all: turtleGraphics turtleGraphicsSimple turtleGraphicsDebug

turtleGraphics: info turtle.o
	$(CC) $(OBJ) -lm -o ./build/$(EXEC)

turtleGraphicsSimple: info simple/turtle.o
	$(CC) $(SIMPLE_OBJ) -lm -o ./build/$(EXEC)-simple

turtleGraphicsDebug: info debug/turtle.o
	$(CC) $(DEBUG_OBJ) -lm -o ./build/$(EXEC)-debug

test: turtleGraphicsDebug test.o
	$(CC) $(TEST_OBJ) -lm -o ./build/test
	./build/test
	if command -v valgrind; then \
		valgrind ./build/test; \
	fi

turtle.o:	makedirs \
			tools.o \
			linkedList.o \
			fileIO.o \
			effects.o \
			turtle.c \
			turtle.h
	$(CC) $(CFLAGS) -o ./objects/turtle.o -c ./turtle.c \
					$(DEFAULT_DEFINES)

simple/turtle.o:	makedirs \
					simple/tools.o \
					simple/linkedList.o \
					simple/fileIO.o \
					simple/effects.o \
					turtle.c \
					turtle.h
	$(CC) $(CFLAGS) -o ./simple/turtle.o -c ./turtle.c \
					$(DEFAULT_DEFINES) \
					-DSIMPLE=1

debug/turtle.o:		makedirs \
					debug/tools.o \
					debug/linkedList.o \
					debug/fileIO.o \
					debug/effects.o \
					turtle.c \
					turtle.h
	$(CC) $(DFLAGS) -o ./debug/turtle.o -c ./turtle.c \
					$(DEFAULT_DEFINES) \
					-DDEBUG=1

effects.o: makedirs effects.c effects.h
	$(CC) $(CFLAGS) -o ./objects/effects.o -c ./effects.c

simple/effects.o: makedirs effects.c effects.h
	$(CC) $(CFLAGS) -o ./simple/effects.o -c ./effects.c

debug/effects.o: makedirs effects.c effects.h
	$(CC) $(DCFLAGS) -o ./debug/effects.o -c ./effects.c

fileIO.o:	makedirs \
			tools.o \
			linkedList.o \
			fileIO.c \
			fileIO.h
	$(CC) $(CFLAGS) -o ./objects/fileIO.o -c ./fileIO.c

simple/fileIO.o:	makedirs \
					simple/tools.o \
					simple/linkedList.o \
					fileIO.c \
					fileIO.h
	$(CC) $(CFLAGS) -o ./simple/fileIO.o -c ./fileIO.c

debug/fileIO.o:	makedirs \
				tools.o \
				fileIO.c \
				fileIO.h
	$(CC) $(DCFLAGS) -o ./debug/fileIO.o -c ./fileIO.c

linkedList.o:	makedirs \
				linkedList.c \
				linkedList.h
	$(CC) $(CFLAGS) -o ./objects/linkedList.o -c ./linkedList.c

simple/linkedList.o:	makedirs \
						linkedList.c \
						linkedList.h
	$(CC) $(CFLAGS) -o ./simple/linkedList.o -c ./linkedList.c

debug/linkedList.o:	makedirs \
					linkedList.c \
					linkedList.h
	$(CC) $(DCFLAGS) -o ./debug/linkedList.o -c ./linkedList.c

test.o:	makedirs \
		tools.o \
		linkedList.o \
		fileIO.o \
		test.c \
		test.h
	$(CC) $(DFLAGS) -o ./debug/test.o -c ./test.c

tools.o: makedirs tools.c tools.h
	$(CC) $(CFLAGS) -o ./objects/tools.o -c ./tools.c

simple/tools.o: makedirs tools.c tools.h
	$(CC) $(CFLAGS) -o ./simple/tools.o -c ./tools.c

debug/tools.o: makedirs tools.c tools.h
	$(CC) $(DCFLAGS) -o ./debug/tools.o -c ./tools.c

docs:
	@bash -c '{ cd ../docs || exit; } && pdflatex AssignmentDoc.tex'

info:
	@echo =====[Makefile Variables]=====
	@echo "CC            = $(CC)"
	@echo "CFLAGS        = $(CFLAGS)"
	@echo "DCFLAGS       = $(DCFLAGS)"
	@echo "COMPILER_INFO = $(COMPILER_INFO)"
	@echo "DATE          = $(DATE)"
	@echo "MOD_DATE      = $(MOD_DATE)"
	@echo "USER          = $(USER)"
	@echo "HOST          = $(HOST)"
	@echo "OBJ           = $(OBJ)"
	@echo "SIMPLE_OBJ    = $(SIMPLE_OBJ)"
	@echo "DEBUG_OBJ     = $(DEBUG_OBJ)"
	@echo "EXEC          = $(EXEC)"
	@echo

makedirs:
	@if [ ! -e ./build ]; then \
		mkdir -p ./build; \
	fi
	@if [ ! -e ./objects ]; then \
		mkdir -p ./objects; \
	fi
	@if [ ! -e ./simple ]; then \
		mkdir -p ./simple; \
	fi
	@if [ ! -e ./debug ]; then \
		mkdir -p ./debug; \
	fi

clean:
	@rm -rfv 	./build \
				./objects \
				./simple \
				./debug \
				../docs/AssignmentDoc.pdf \
				*.log

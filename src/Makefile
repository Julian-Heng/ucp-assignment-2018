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

# Getting user information
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

SOURCE     = effects.c fileIO.c linkedList.c tools.c
OBJ        = $(SOURCE:%.c=objects/%.o)
SIMPLE_OBJ = $(SOURCE:%.c=simple/%.o)
DEBUG_OBJ  = $(SOURCE:%.c=debug/%.o)
EXEC = turtle

all: info makedirs $(OBJ) objects/turtle.o
	$(CC) $(OBJ) ./objects/turtle.o -lm -o ./build/$(EXEC)

simple: info makedirs $(SIMPLE_OBJ) simple/turtle.o
	$(CC) $(SIMPLE_OBJ) ./simple/turtle.o -lm -o ./build/$(EXEC)-simple

debug: info makedirs $(DEBUG_OBJ) debug/turtle.o
	$(CC) $(DEBUG_OBJ) ./debug/turtle.o -lm -o ./build/$(EXEC)-debug

test: makedirs debug
	@echo =====[Test]=====
	$(CC) $(DCFLAGS) -o ./debug/test.o -c ./test.c
	$(CC) $(DEBUG_OBJ) ./debug/test.o -lm -o ./build/test
	./build/test
	if command -v valgrind; then \
		valgrind ./build/test; \
	fi
	@echo

everything: info all simple debug

$(OBJ): makedirs
	$(CC) $(CFLAGS) -o ./$@ -c ./$(@:objects/%.o=%.c)

$(SIMPLE_OBJ): makedirs
	$(CC) $(CFLAGS) -o ./$@ -c ./$(@:simple/%.o=%.c)

$(DEBUG_OBJ): makedirs
	$(CC) $(DCFLAGS) -o ./$@ -c ./$(@:debug/%.o=%.c)

objects/turtle.o: makedirs $(OBJ)
	$(CC) $(CFLAGS) -o ./$@ -c ./turtle.c \
					$(DEFAULT_DEFINES)

simple/turtle.o: makedirs $(SIMPLE_OBJ)
	$(CC) $(CFLAGS) -o ./$@ -c ./turtle.c \
					$(DEFAULT_DEFINES) \
					-DSIMPLE=1 \

debug/turtle.o: makedirs $(DEBUG_OBJ)
	$(CC) $(DCFLAGS) -o ./$@ -c ./turtle.c \
					 $(DEFAULT_DEFINES) \
					 -DDEBUG=1 \

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
	@echo "SOURCE        = $(SOURCE)"
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

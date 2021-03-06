CC=gcc
CFLAGS=-O3 -g
local_CFLAGS=-O3
DEPS= cpuSim.h control.h registers.h

# %.o: %.c means everything matching the pattern filename.o depends on filename.c
# $(DEPS) means it also depends on the dependencies listed in the macro DEPS
# $(CC) means use compiler in macro CC
# -c means generate object file
# -o $@ means the object file for file.c should be named file.o
# $< is first item in DEPS
# $@ is left side of colon, $^ is right side

.PHONY: all clean

#all: local_CFLAGS=$(filter-out -DDEBUG,$(CFLAGS))
all: executable

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

executable: main.o cpuSim.o control.o stages.o
	$(CC) -o cpuSim $^

clean:
	#local_CFLAGS=$(filter-out -DDEBUG,$(CFLAGS))
	rm *.exe *.o

CC=gcc
SIM_PARAMS=-D L=48
CFLAGS=-std=c11 -Wall -Wextra $(SIM_PARAMS)
LDFLAGS=-lm
GL_LDFLAGS=-lGL -lglfw

# Files
TARGETS=tiny_ising

# Rules
all: $(TARGETS)

tiny_ising: tiny_ising.o ising.o wtime.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

demo: demo.o ising.o wtime.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(GL_LDFLAGS)

clean:
	rm -f $(TARGETS) *.o

.PHONY: clean all


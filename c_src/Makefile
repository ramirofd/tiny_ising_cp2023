CC=gcc
SIM_PARAMS=-DL=2048 -DMETRIC -DBLOCK=16
# SIM_PARAMS=-DL=512 -DOUTPUT_SIM_RESULT -DBLOCK=16
OPTIM_FLAGS=-O2 -march=native -ftree-vectorize -fopt-info-vec -fopt-info-missed-vec -fno-strict-aliasing -fopenmp
CFLAGS=-std=c11 -Wall -Wextra $(OPTIM_FLAGS) $(SIM_PARAMS)
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


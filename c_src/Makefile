CC=gcc
SIM_PARAMS=-DL=512 -DMETRIC
# SIM_PARAMS=-DL=512 -DOUTPUT_SIM_RESULT
OPTIM_FLAGS=-O3 -march=native -ffast-math -ftree-vectorize -fopt-info-vec
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


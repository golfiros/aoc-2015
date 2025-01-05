DIR_INP = .input
DIR_BIN = .build

dir_guard = @mkdir -p $(@D)

CC = gcc
CFLAGS = -O3 -Wpedantic -std=c23

LIBS = -lm
BINARIES = $(patsubst %.c, $(DIR_BIN)/%, $(wildcard *.c))
INPUTS = $(patsubst %.c, $(DIR_INP)/%, $(wildcard *.c))

default: $(BINARIES) $(INPUTS)

.PHONY: $(patsubst %.c, /%, $(wildcard *.c)) clean default

day%: $(DIR_BIN)/day% $(DIR_INP)/day%
	@cat $(DIR_INP)/$@ | $(DIR_BIN)/$@

clean: 
	@rm -rf $(DIR_BIN)
	@rm -rf $(DIR_INP)

$(DIR_BIN)/day%: day%.c
	$(dir_guard)
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(DIR_INP)/day%:
	$(dir_guard)
	@curl -s -o $(DIR_INP)/$(@F) --cookie .cookie https://adventofcode.com/2015/day/$*/input

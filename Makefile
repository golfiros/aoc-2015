DIR_SRC = src
DIR_INP = input
DIR_BIN = build

dir_guard = @mkdir -p $(@D)

CC = gcc
CFLAGS = -g -Wpedantic

LIBS = -lm
BINARIES = $(patsubst $(DIR_SRC)/%, $(DIR_BIN)/%, $(wildcard $(DIR_SRC)/*))

default: $(BINARIES)

.PHONY: $(patsubst $(DIR_SRC)/%, /%, $(wildcard $(DIR_SRC)/*)) clean default

day%: $(DIR_BIN)/day%
	@./$< $(DIR_INP)/$@

clean: 
	@rm -rf $(DIR_BIN)

.SECONDEXPANSION:
$(DIR_BIN)/day%: $$(shell find $(DIR_SRC)/day% -type f)
	$(dir_guard)
	@$(CC) $(CFLAGS) -I$(DIR_SRC)/$(@F) -o $@ $^ $(LIBS)

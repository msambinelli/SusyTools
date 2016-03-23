#!/usr/bin/make -f

DISCIPLINA := mc999
TAREFA := 00

MKFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOTDIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE))))

.PHONY: all testes

CC = gcc
CPPFLAGS =
CFLAGS = -O3 -std=c99 -Werror -Wall -pedantic
CXXFLAGS =
LDFLAGS = -O3
LDLIBS =

SOURCES   = ./lab$(TAREFA).c
OBJECTS   = $(SOURCES:.c=.o)
SOL       = ./lab$(TAREFA)

TEST_IN   = $(wildcard dados/*.in)
TEST_OUT  = $(wildcard dados/*.out)
TEST_RES  = $(wildcard dados/*.res)

TESTER    = testador.sh

all: $(SOL) testes

$(SOL): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LOADLIBES) $(LDLIBS) -o $(SOL)

clean:
	rm -f $(SOL) $(OBJECTS) $(TEST_OUT)

distclean: clean
	rm -rf dados $(TESTER)

$(TESTER):
	@curl -o $@ -ks https://susy.ic.unicamp.br:9999/$(DISCIPLINA)/$(TAREFA)/aux/$@; \
	if [ "$$(file $@)" != *"Bourne-Again shell script"* ]; then \
		curl -o $@ -ks https://raw.githubusercontent.com/tiagofalcao/SusyTools/master/solucao/$@; \
	fi

testes: $(SOL) $(TESTER)
	@ bash $(TESTER) $(DISCIPLINA) $(TAREFA)

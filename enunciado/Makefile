#!/usr/bin/make -f
MKFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOTDIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE))))

.PHONY: enunciado tarefa testes clean distclean all Enunciado EnunciadoAberto SusyCoord SusyFinal

CC = gcc
CPPFLAGS =
CFLAGS = -O3 -std=c99 -Werror -Wall -Wextra -Wpedantic
CXXFLAGS =
LDFLAGS = -O3
LDLIBS =

SOURCES   = $(wildcard sols/*.c)
OBJECTS   = $(SOURCES:.c=.o)
SOL       = ./sols/sol


TEST_IN   = $(wildcard dados/*.in)
TEST_RES   = $(TEST_IN:.in=.res)

all: enunciado Enunciado EnunciadoAberto SusyCoord SusyFinal

$(SOL): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LOADLIBES) $(LDLIBS) -o $(SOL)

%.res: %.in $(SOL)
	@echo -n "Gerando saída para $<: "; \
        if output=$$(/usr/bin/time -f "%Us" $(SOL) <$< >$@); then \
	  echo $$output; \
        else \
          tail $@; \
          rm $@; \
          exit 1; \
        fi

tarefa: tarefa.cfg
	@echo "Lendo tarefa.cfg"
	$(eval ID := $(shell grep '^TASKID:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval TITLE := $(shell grep '^TASK_COMMENT:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval FINAL_TIME := $(shell grep '^FINAL_TIME:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval FINAL_TIME := $(subst :, ,$(FINAL_TIME)))
	$(eval FINAL_DATE := $(shell grep '^FINAL_DATE:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval FINAL_DATE := $(subst /, ,$(FINAL_DATE)))
	$(eval DEADLINE := $(word 3, $(FINAL_DATE))/$(word 2, $(FINAL_DATE))/$(word 1, $(FINAL_DATE)) às $(word 1, $(FINAL_TIME)):$(word 2, $(FINAL_TIME)):$(word 3, $(FINAL_TIME)))
	$(eval PAGE := $(shell grep '^TASK_DESCRIPTION_PAGE:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval MAX_SUBMISSIONS := $(shell grep '^MAX_SUBMISSIONS:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))

enunciado: tarefa testes enunciado.md md_head.html md_tail.html
	@echo "Criando $(PAGE) da Tarefa $(ID)"
	@cat enunciado.md | \
	sed 's/!TESTE!\([0-9]\+\)!/###### Teste \1\n\n* [Entrada](dados\/arq\1.in)\n\n    ```\ninclude("dados\/arq\1.in")\n    ```\n\n* [Saída](dados\/arq\1.res)\n\n    ```\ninclude("dados\/arq\1.res")\n    ```\n\n/g' | \
	awk '/</ || /\?>/ {next;} /include/ { if (match($$0,"\".*\"")){ f = substr($$0,RSTART+1,RLENGTH-2); while ( getline < f ){ print; } } next; } {print;}' | \
	curl https://api.github.com/markdown/raw -s -XPOST -H Content-Type:text/x-markdown --data-binary @- | \
	cat md_head.html - md_tail.html | \
	sed "s#!ID!#$(ID)#g;s#!TITLE!#$(TITLE)#g;s#!DEADLINE!#$(DEADLINE)#g;s#!MAX_SUBMISSIONS!#$(MAX_SUBMISSIONS)#g;" >$(PAGE)

clean: tarefa
	rm -f $(SOL) $(OBJECTS) $(PAGE)
	rm -rf SusyCoord Enunciado EnunciadoAberto SusyFinal

distclean: clean
	rm -f $(TEST_RES)

testes: $(TEST_IN) $(TEST_RES)

SusyCoord: tarefa enunciado testes
	@echo "Criando pacote para coordenação: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	mkdir -p ./$@/$(ID)/sols ; \
	cp -fr ./sols/*.c ./$@/$(ID)/sols/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	cp -fr ./dados/* ./$@/$(ID)/dados/ ; \
	OPEN=$$(grep '^TEST_CASES:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//;s/\]//g;'); \
	sed -e "s/^TASK_DESCRIPTION_PAGE:.*$$/TASK_DESCRIPTION_PAGE: None/;s/^TEST_CASES:.*$$/#TEST_CASES: []/;s/HIDDEN_TEST_CASES: *\[/HIDDEN_TEST_CASES: $$OPEN,/" tarefa.cfg >./$@/$(ID)/tarefa.cfg; \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

Enunciado: tarefa enunciado testes
	@echo "Compactando Enunciado: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	[ -d "./aux" ] && cp -r ./aux ./$@/$(ID)/ ; \
	[ -d "./images" ] && cp -r ./images ./$@/$(ID)/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	cp -fr ./dados/* ./$@/$(ID)/dados/ ; \
	cp $(PAGE) ./$@/$(ID)/ ; \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

EnunciadoAberto: tarefa enunciado testes
	@echo "Compactando Enunciado: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	[ -d "./aux" ] && cp -r ./aux ./$@/$(ID)/ ; \
	[ -d "./images" ] && cp -r ./images ./$@/$(ID)/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	for i in $$(grep '^TEST_CASES:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//;s/[[,"\]\+]\?/ /g;'); do cp ./dados/arq$$i.* ./$@/$(ID)/dados/; done; \
	cp $(PAGE) ./$@/$(ID)/ ; \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

SusyFinal: tarefa enunciado testes
	@echo "Criando pacote para alunos: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	[ -d "./aux" ] && cp -r ./aux ./$@/$(ID)/ ; \
	[ -d "./images" ] && cp -r ./images ./$@/$(ID)/ ; \
	mkdir -p ./$@/$(ID)/sols ; \
	cp -fr ./sols/*.c ./$@/$(ID)/sols/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	cp -fr ./dados/* ./$@/$(ID)/dados/ ; \
	cp $(PAGE) ./$@/$(ID)/ ; \
	cp tarefa.cfg ./$@/$(ID)/ ; \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

#!/usr/bin/make -f
MKFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOTDIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE))))

.PHONY: enunciado tarefa testes clean distclean all EnunciadoCoord EnunciadoAberto SusyCoord SusyFinal
.SUFFIX: .pre .in .out .res

CC = gcc
CPPFLAGS =
CFLAGS = -O3 -std=c99 -Werror -Wall -Wextra -Wpedantic
CXXFLAGS =
LDFLAGS = -O3
LDLIBS =

SOURCES   = $(wildcard sols/*.c)
OBJECTS   = $(SOURCES:.c=.o)
SOL       = sols/sol

GENSOURCES   = $(wildcard gen/*.c)
GENOBJECTS   = $(GENSOURCES:.c=.o)
GENERATOR = gen/gen

TEST_IN   = $(wildcard dados/*.in)
TEST_OUT  = $(TEST_IN:.in=.out)
TEST_RES  = $(TEST_IN:.in=.res)
TEST_DIFF = $(TEST_IN:.in=.diff)

TEST_PRE   = $(wildcard dados/*.pre)
TEST_PRE_IN = $(TEST_PRE:.pre=.in)
TEST_PRE_OUT = $(TEST_PRE:.pre=.out)
TEST_PRE_RES = $(TEST_PRE:.pre=.res)

define INCLUDE_PY
import re
import sys
inc = re.compile('(?P<pre>.*)include\(\"(?P<file>[^\"]+)\"\)(?P<pos>.*)')
def printfile(file):
	for line in file:
		m = inc.match(line)
		if m is None:
			sys.stdout.write(line)
		else:
			sys.stdout.write(m.group('pre'))
			printfile(open(m.group('file'), 'rb'))
			sys.stdout.write(m.group('pos'))
printfile(sys.stdin)
endef
export INCLUDE_PY

define HEAD_HTML
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv='content-type' content='text/html; charset=utf-8'>
	<title>Laboratório !ID!</title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
	<link rel='stylesheet' href='https://sindresorhus.com/github-markdown-css/github-markdown.css'>
	<style>
	    .markdown-body {
		box-sizing: border-box;
		min-width: 200px;
		max-width: 980px;
		margin: 0 auto;
		padding: 45px;
	    }
	    .markdown-body h6 { margin-bottom: 0; }
	    .markdown-body h6 + ul li {
		display: inline-block;
		width: 100%;
		max-width: 426px;
		vertical-align: top;
	    }
	</style>
</head>
<body>
	<article class='markdown-body'>
endef
export HEAD_HTML

define TAIL_HTML
</article>
</body>
</html>
endef
export TAIL_HTML

all: testes enunciado EnunciadoCoord EnunciadoAberto SusyCoord SusyFinal

sols/%.o: sols/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

sols/%: sols/%.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

gen/%.o: gen/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

gen/%: gen/%.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

dados/%.in: dados/%.pre | $(GENERATOR)
	@echo "Gerando teste com $<"; \
	PRE="$<"; \
	if ./$(GENERATOR) $< "$${PRE%.pre}.in" "$${PRE%.pre}.res"; then \
		echo "[ OK ] "; \
	else \
		echo "[FAIL]"; \
		rm -f "$${PRE%.pre}.in" "$${PRE%.pre}.res"; \
	fi

dados/%.res: dados/%.in

dados/%.out: dados/%.in $(SOL)
	@echo -n "Gerando saída para $<: "; \
  if output=$$(/usr/bin/time -f "%Us" ./$(SOL) <$< 2>&1 >$@); then \
		OUT="$@"; \
		if [ -f "$${OUT%.out}.res" ]; then \
			if diff --suppress-common-lines "$${OUT}" "$${OUT%.out}.res" > "$${OUT%.out}.diff"; then \
				echo -n "[ OK ] "; \
				echo $$output; \
			else \
				echo "[FAIL]"; \
				echo "See $${OUT%.out}.diff:"; \
				tail -n 20 "$${OUT%.out}.diff"; \
		    rm $@; \
				exit 1; \
			fi; \
		else \
			echo $$output; \
		fi; \
  else \
    tail $@; \
    rm $@; \
    exit 1; \
  fi;

testes: $(TEST_PRE_IN) $(TEST_PRE_RES) $(TEST_PRE_OUT) $(TEST_OUT)

tarefa: tarefa.cfg
	@echo "Lendo tarefa.cfg"
	$(eval ID := $(shell grep '^TASKID:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval TITLE := $(shell grep '^TASK_COMMENT:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval FINAL_TIME := $(shell grep '^FINAL_TIME:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval FINAL_TIME := $(subst :, ,$(FINAL_TIME)))
	$(eval FINAL_DATE := $(shell grep '^FINAL_DATE:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval FINAL_DATE := $(subst /, ,$(FINAL_DATE)))
	$(eval DEADLINE := $(word 3, $(FINAL_DATE))/$(word 2, $(FINAL_DATE))/$(word 1, $(FINAL_DATE)) às $(word 1, $(FINAL_TIME)):$(word 2, $(FINAL_TIME)):$(word 3, $(FINAL_TIME)))
	$(eval COORD_FINAL_TIME := $(shell grep '^COORD_FINAL_TIME:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval COORD_FINAL_TIME := $(subst :, ,$(COORD_FINAL_TIME)))
	$(eval COORD_FINAL_DATE := $(shell grep '^COORD_FINAL_DATE:' tarefa.cfg | cut -f 1 -d'#' | cut -f2 -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval COORD_FINAL_DATE := $(subst /, ,$(COORD_FINAL_DATE)))
	$(eval COORD_DEADLINE := $(word 3, $(COORD_FINAL_DATE))/$(word 2, $(COORD_FINAL_DATE))/$(word 1, $(COORD_FINAL_DATE)) às $(word 1, $(COORD_FINAL_TIME)):$(word 2, $(COORD_FINAL_TIME)):$(word 3, $(COORD_FINAL_TIME)))
	$(eval PAGE := $(shell grep '^TASK_DESCRIPTION_PAGE:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))
	$(eval MAX_SUBMISSIONS := $(shell grep '^MAX_SUBMISSIONS:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//'))

md_head.html:
	@echo -n "$${HEAD_HTML}" >$@

md_tail.html:
	@echo -n "$${TAIL_HTML}" >$@

md_content.md: testes enunciado.md
	@echo "Processando enunciado.md"; \
	cat enunciado.md | \
	sed 's/!TESTE!\([0-9]\+\)!/###### Teste \1\n\n* [Entrada](dados\/arq\1.in)\n\n    ```\ninclude("dados\/arq\1.in")\n    ```\n\n* [Saída](dados\/arq\1.res)\n\n    ```\ninclude("dados\/arq\1.out")\n    ```\n\n/g'  >$@

md_content.html: md_content.md
	@echo "Gerando md_content.html"; \
	cat $< | curl https://api.github.com/markdown/raw -s -XPOST -H Content-Type:text/x-markdown --data-binary @- >$@

enunciado: tarefa md_head.html md_content.html md_tail.html
	@echo "Criando $(PAGE) da Tarefa $(ID)"; \
	cat md_head.html md_content.html md_tail.html | \
	python -c "$${INCLUDE_PY}" | \
	sed "s#!ID!#$(ID)#g;s#!TITLE!#$(TITLE)#g;s#!MAX_SUBMISSIONS!#$(MAX_SUBMISSIONS)#g;" >$(PAGE)

clean: tarefa
	rm -f $(SOL) $(OBJECTS) $(PAGE) $(TEST_OUT) $(TEST_DIFF) $(GENERATOR) md_head.html md_content.md md_content.html md_tail.html
	rm -rf SusyCoord EnunciadoCoord EnunciadoAberto SusyFinal

distclean: clean
	rm -f $(TEST_PRE_IN) $(TEST_PRE_RES)

SusyCoord: tarefa enunciado testes
	@echo "Criando pacote para coordenação: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	mkdir -p ./$@/$(ID)/sols ; \
	cp -fr ./sols/*.c ./$@/$(ID)/sols/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	cp -fr ./dados/*.in ./$@/$(ID)/dados/ ; \
	OPEN=$$(grep '^TEST_CASES:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//;s/\]//g;'); \
	sed -e "s/^TASK_DESCRIPTION_PAGE:.*$$/TASK_DESCRIPTION_PAGE: None/;\
	s/^TEST_CASES:.*$$/#TEST_CASES: []/;\
	s/HIDDEN_TEST_CASES: *\[/HIDDEN_TEST_CASES: $$OPEN,/;\
	s/^START_/#START_/;s/^FINAL_/#FINAL_/;\
	s/^COORD_START_/START_/;s/^COORD_FINAL_/FINAL_/;" tarefa.cfg >./$@/$(ID)/tarefa.cfg; \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

EnunciadoCoord: tarefa enunciado testes
	@echo "Compactando Enunciado: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	[ -d "./aux" ] && cp -r ./aux ./$@/$(ID)/ ; \
	[ -d "./images" ] && cp -r ./images ./$@/$(ID)/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	cp -fr ./dados/*.in ./$@/$(ID)/dados/ ; \
	for i in ./dados/*.out; do r=$$(basename $$i); cp -fr "$$i" "./$@/$(ID)/dados/$${r%.out}.res"; done; \
	sed "s#!DEADLINE!#$(COORD_DEADLINE)#g;" $(PAGE) >./$@/$(ID)/$(PAGE); \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

EnunciadoAberto: tarefa enunciado testes
	@echo "Compactando Enunciado: $@/$(ID).zip"; \
	rm -rf ./$@ ; \
	mkdir -p ./$@/$(ID) ; \
	[ -d "./aux" ] && cp -r ./aux ./$@/$(ID)/ ; \
	[ -d "./images" ] && cp -r ./images ./$@/$(ID)/ ; \
	mkdir -p ./$@/$(ID)/dados ; \
	for i in $$(grep '^TEST_CASES:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//;s/[[,"\]\+]\?/ /g;'); do cp ./dados/arq$$i.in ./$@/$(ID)/dados/; cp ./dados/arq$$i.out ./$@/$(ID)/dados/arq$$i.res;  done; \
	sed "s#!DEADLINE!#$(DEADLINE)#g;" $(PAGE) >./$@/$(ID)/$(PAGE); \
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
	cp -fr ./dados/*.in ./$@/$(ID)/dados/ ; \
	sed "s#!DEADLINE!#$(DEADLINE)#g;" $(PAGE) >./$@/$(ID)/$(PAGE); \
	sed -e "s/^COORD_/#COORD_/;" tarefa.cfg >./$@/$(ID)/tarefa.cfg; \
	cd ./$@/ && zip -q -r ./$(ID).zip $(ID) ; \
	cd ..; rm -rf ./$@/$(ID)

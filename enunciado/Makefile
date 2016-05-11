#!/usr/bin/make -f
MKFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOTDIR := $(shell dirname "$(MKFILE)")
DIRNAME := $(shell basename "$(ROOTDIR)")
MAKEFLAGS += --no-builtin-rules

CC = gcc
CPPFLAGS =
CFLAGS = -O3 -std=c99 -Werror -Wall -Wextra -pedantic -Wunused-result -Wno-error=unused-result
CXXFLAGS =
LDFLAGS = -O3
LDLIBS =

SOURCES   = $(wildcard sols/*.c)
OBJECTS   = $(SOURCES:.c=.o)
SOL       = sols/sol

GENSOURCES   = $(wildcard gen/*.c)
GENOBJECTS   = $(GENSOURCES:.c=.o)
GENERATOR = gen/gen

.PRECIOUS: $(SOL) $(GENERATOR)

TEST_PRE   = $(wildcard dados/*.pre)
TEST_PRE_IN = $(TEST_PRE:.pre=.in)
TEST_PRE_OUT = $(TEST_PRE:.pre=.out)
TEST_PRE_RES = $(TEST_PRE:.pre=.res)

TEST_IN   = $(filter-out $(TEST_PRE_IN),$(wildcard dados/*.in))
TEST_OUT  = $(TEST_IN:.in=.out)
TEST_RES  = $(TEST_IN:.in=.res)
TEST_DIFF = $(TEST_IN:.in=.diff)

.SUFFIX: .pre .in .out .res
.PHONY: enunciado tarefa testes clean distclean all EnunciadoCoord EnunciadoAberto SusyCoord SusyFinal

define INCLUDE_PY
import re
import sys
inc = re.compile('(?P<pre>.*)include\(\"(?P<file>[^\"]+)\"\)(?P<pos>.*)')
def printfile(file):
	for line in file:
		line = str(line)
		m = inc.match(line)
		if m is None:
			sys.stdout.write(line)
		else:
			sys.stdout.write(m.group('pre'))
			printfile(open(m.group('file'), 'r'))
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

$(SOL): $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SOURCES) $(LOADLIBES) $(LDLIBS) -o $@

$(GENERATOR): $(GENSOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(GENSOURCES) $(LOADLIBES) $(LDLIBS) -o $@

$(TEST_PRE_IN): %.in: %.pre $(GENERATOR)
	@printf "Gerando teste com $<: "; \
	PRE="$<"; \
	if output=$$(./$(GENERATOR) $< "$${PRE%.pre}.in" "$${PRE%.pre}.res"); then \
		echo "[ OK ]"; \
	else \
		echo "[FAIL]"; \
		rm -f "$${PRE%.pre}.in" "$${PRE%.pre}.res"; \
		[ -z "$$output" ] || echo "$$output"; \
		exit 1; \
	fi;

$(TEST_PRE_RES): %.res: %.in

$(TEST_OUT) $(TEST_PRE_OUT): %.out: %.in $(SOL) /usr/bin/time
	@printf "Gerando saida para $<: "; \
	if [ ! -x "$(SOL)" ]; then \
		echo "$(SOL) is not a executable"; \
	fi; \
	if output=$$(/usr/bin/time -p ./$(SOL) <$< 2>&1 >$@ | grep real | awk '{{print $$2}}'); then \
		OUT="$@"; \
		RES="$${OUT%.out}.res"; \
		WOUT="$${OUT%.out}.wrong_out"; \
		DIFF="$${OUT%.out}.diff"; \
		if [ -f "$${RES}" ]; then \
			rm -f "$${WOUT}" "$${DIFF}"; \
			if diff --suppress-common-lines "$${OUT}" "$${RES}" > "$${DIFF}"; then \
				printf "[ OK ] %s\n" "$$output"; \
				rm "$${DIFF}"; \
			else \
				printf "[FAIL] See %s:\n" "$${DIFF}"; \
				tail -n 20 "$${DIFF}"; \
				mv $@ "$${WOUT}"; \
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
	@printf "%s" "$${HEAD_HTML}" >$@

md_tail.html:
	@printf "%s" "$${TAIL_HTML}" >$@

md_content.md: testes enunciado.md
	@echo "Processando enunciado.md"; \
	cat enunciado.md | \
	sed 's/!TESTE!\([0-9][0-9]*\)!/###### Teste \1\n\n* [Entrada](dados\/arq\1.in)\n\n    ```\ninclude("dados\/arq\1.in")\n    ```\n\n* [Saída](dados\/arq\1.res)\n\n    ```\ninclude("dados\/arq\1.out")\n    ```\n\n/g'  >$@

md_content.html: md_content.md
	@echo "Gerando md_content.html"; \
	cat $< | \
	/usr/bin/env python -c "$${INCLUDE_PY}" | \
	curl https://api.github.com/markdown/raw -s -XPOST -H Content-Type:text/x-markdown --data-binary @- >$@

enunciado: tarefa md_head.html md_content.html md_tail.html
	@echo "Criando $(PAGE) da Tarefa $(ID)"; \
	cat md_head.html md_content.html md_tail.html | \
	sed "s#!ID!#$(ID)#g;s#!TITLE!#$(TITLE)#g;s#!MAX_SUBMISSIONS!#$(MAX_SUBMISSIONS)#g;" >$(PAGE)

clean: tarefa
	rm -f $(SOL) $(OBJECTS) $(PAGE) $(TEST_OUT) $(TEST_DIFF) md_head.html md_content.md md_content.html md_tail.html
	rm -rf SusyCoord EnunciadoCoord EnunciadoAberto SusyFinal

distclean: clean
	rm -f $(TEST_PRE_IN) $(TEST_PRE_RES) $(GENERATOR)

SusyCoord: tarefa enunciado testes
	@TMPDIR=$$(mktemp -d); \
	cd "$(ROOTDIR)" ; \
	echo "Criando pacote para coordenação: $@/$(ID).zip ($${TMPDIR}/$(ID))"; \
	mkdir -p ./$@/ ; \
	rm -rf ./$@/$(ID).zip ; \
	mkdir -p $${TMPDIR}/$(ID) ; \
	[ -d "./aux" ] && cp -R ./aux $${TMPDIR}/$(ID)/ ; \
	mkdir -p $${TMPDIR}/$(ID)/sols ; \
	cp -fR ./sols/*.c $${TMPDIR}/$(ID)/sols/ ; \
	mkdir -p $${TMPDIR}/$(ID)/dados ; \
	cp -fR ./dados/*.in $${TMPDIR}/$(ID)/dados/ ; \
	OPEN=$$(grep '^TEST_CASES:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed -e 's/^ *//;s/ *$$//;s/\]//g;'); \
	sed -e "s/^TASK_DESCRIPTION_PAGE:.*$$/TASK_DESCRIPTION_PAGE: None/;\
	s/^TEST_CASES:.*$$/#TEST_CASES: []/;\
	s/HIDDEN_TEST_CASES: *\[/HIDDEN_TEST_CASES: $$OPEN,/;\
	s/^START_/#START_/;s/^FINAL_/#FINAL_/;\
	s/^COORD_START_/START_/;s/^COORD_FINAL_/FINAL_/;" tarefa.cfg >$${TMPDIR}/$(ID)/tarefa.cfg; \
	cd $${TMPDIR}/ ; \
	zip -q -r ./$(ID).zip $(ID) ; \
	cd "$(ROOTDIR)" ; \
	mv $${TMPDIR}/$(ID).zip ./$@/$(ID).zip ;

EnunciadoCoord: tarefa enunciado testes
	@TMPDIR=$$(mktemp -d); \
	cd "$(ROOTDIR)" ; \
	echo "Compactando Enunciado: $@/$(ID).zip ($${TMPDIR}/$(ID))"; \
	mkdir -p ./$@/ ; \
	rm -rf ./$@/$(ID).zip ; \
	mkdir -p $${TMPDIR}/$(ID) ; \
	[ -d "./aux" ] && cp -R ./aux $${TMPDIR}/$(ID)/ ; \
	mkdir -p $${TMPDIR}/$(ID)/dados ; \
	cp -fR ./dados/*.in $${TMPDIR}/$(ID)/dados/ ; \
	for i in ./dados/*.in ; do cp $${i%.in}.out $${TMPDIR}/$(ID)/$${i%.in}.res;  done; \
	EXT="png"; find . -mindepth 1 -maxdepth 1 -type f -name '*.png' -exec cp {} $${TMPDIR}/$(ID)/ \; ; \
	sed "s#!DEADLINE!#$(COORD_DEADLINE)#g;" $(PAGE) >$${TMPDIR}/$(ID)/$(PAGE); \
	cd $${TMPDIR}/ ; \
	zip -q -r ./$(ID).zip $(ID) ; \
	cd "$(ROOTDIR)" ; \
	mv $${TMPDIR}/$(ID).zip ./$@/$(ID).zip ;

EnunciadoAberto: tarefa enunciado testes
	@TMPDIR=$$(mktemp -d); \
	cd "$(ROOTDIR)" ; \
	echo "Compactando Enunciado: $@/$(ID).zip ($${TMPDIR}/$(ID))"; \
	mkdir -p ./$@/ ; \
	rm -rf ./$@/$(ID).zip ; \
	mkdir -p $${TMPDIR}/$(ID) ; \
	[ -d "./aux" ] && cp -R ./aux $${TMPDIR}/$(ID)/ ; \
	mkdir -p $${TMPDIR}/$(ID)/dados ; \
	for i in $$(grep '^TEST_CASES:' tarefa.cfg | cut -f 1 -d'#' | cut -f2- -d':' | sed 's/\[/ /g;' | sed 's/\]/ /g;' | sed 's/[", ][", ]*/ /g;'); do cp ./dados/arq$$i.in $${TMPDIR}/$(ID)/dados/; cp ./dados/arq$$i.out $${TMPDIR}/$(ID)/dados/arq$$i.res;  done; \
	EXT="png"; find . -mindepth 1 -maxdepth 1 -type f -name '*.png' -exec cp {} $${TMPDIR}/$(ID)/ \; ; \
	sed "s#!DEADLINE!#$(DEADLINE)#g;" $(PAGE) >$${TMPDIR}/$(ID)/$(PAGE); \
	cd $${TMPDIR}/ ; \
	zip -q -r ./$(ID).zip $(ID) ; \
	cd "$(ROOTDIR)" ; \
	mv $${TMPDIR}/$(ID).zip ./$@/$(ID).zip ;

SusyFinal: tarefa enunciado testes
	@TMPDIR=$$(mktemp -d); \
	cd "$(ROOTDIR)" ; \
	echo "Criando pacote para alunos: $@/$(ID).zip ($${TMPDIR}/$(ID))"; \
	mkdir -p ./$@/ ; \
	rm -rf ./$@/$(ID).zip ; \
	mkdir -p $${TMPDIR}/$(ID) ; \
	[ -d "./aux" ] && cp -R ./aux $${TMPDIR}/$(ID)/ ; \
	mkdir -p $${TMPDIR}/$(ID)/sols ; \
	cp -fR ./sols/*.c $${TMPDIR}/$(ID)/sols/ ; \
	mkdir -p $${TMPDIR}/$(ID)/dados ; \
	cp -fR ./dados/*.in $${TMPDIR}/$(ID)/dados/ ; \
	EXT="png"; find . -mindepth 1 -maxdepth 1 -type f -name '*.png' -exec cp {} $${TMPDIR}/$(ID)/ \; ; \
	sed "s#!DEADLINE!#$(DEADLINE)#g;" $(PAGE) >$${TMPDIR}/$(ID)/$(PAGE); \
	sed -e "s/^COORD_/#COORD_/;" tarefa.cfg >$${TMPDIR}/$(ID)/tarefa.cfg; \
	cd $${TMPDIR}/ ; \
	zip -q -r ./$(ID).zip $(ID) ; \
	cd "$(ROOTDIR)" ; \
	mv $${TMPDIR}/$(ID).zip ./$@/$(ID).zip ;

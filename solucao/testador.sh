#!/bin/bash

# Colors
ESC_SEQ="\x1b["
COL_RESET=$ESC_SEQ"39;49;00m"
COL_RED=$ESC_SEQ"31;01m"
COL_GREEN=$ESC_SEQ"32;01m"
COL_YELLOW=$ESC_SEQ"33;01m"
COL_BLUE=$ESC_SEQ"34;01m"
COL_MAGENTA=$ESC_SEQ"35;01m"
COL_CYAN=$ESC_SEQ"36;01m"
COL_BOLD=$ESC_SEQ";1m"


self="${0##*/}"

DISCIPLINA=$1
shift
TAREFA=$1

echo "Executando os testes..."
erros=0
SUSY="https://susy.ic.unicamp.br:9999/${DISCIPLINA}/${TAREFA}/dados"
mkdir -p ./dados/
for input in $(curl -ks "$SUSY/testes.html" | sed -n '/Testes abertos/,/Testes fechados/p' | sed 's/"/\n/g' | grep '^a' | grep '.in'); do
  in_file="dados/${input}"
  base="${input%.in}"
  printf "${COL_BOLD}%-12s${COL_RESET}" "${base}"
	res="${base}.res"
  res_file="dados/${res}"
	out="${base}.out"
  out_file="dados/${out}"
  touch "${in_file}"
	curl -ks "${SUSY}/${input}" -o "${in_file}"
  touch "${res_file}"
	curl -ks "${SUSY}/${res}" -o "${res_file}"
	./lab${TAREFA} <"${in_file}" &>"${out_file}"
	if diff -q "${res_file}" "${out_file}" &>/dev/null; then
		printf "${COL_GREEN}%-12s${COL_RESET}\n" "OK"
	else
		printf "${COL_RED}%-12s${COL_RESET}\n" "Erro"
    echo ">>> Saida esperada (SuSy):"
    cat "${res_file}"
    echo ">>> Saida do seu programa:"
    cat "${out_file}"
    erros=$(($erros+1))
	fi
done

echo -e "\nTotal de erros encontrados: ${erros}"

exit ${erros}

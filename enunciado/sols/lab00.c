#include <stdio.h>

int main() {
  char v;
  int l = 0;
  int c = 0;

  while ((v = fgetc(stdin)) != EOF) {
    if (v == '\n')
      l++;
    else
      c++;
  }

  printf("Caracteres: %d\n", c);
  printf("Linhas:     %d\n", l);

  return 0;
}

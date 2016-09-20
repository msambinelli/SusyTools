#include <stdio.h>

int isLineBreak(char c);

int main() {
  char v;
  int l = 0;
  int c = 0;

  while ((v = fgetc(stdin)) != EOF) {
    if (isLineBreak(v)) {
      l++;
    } else {
      c++;
    }
  }

  printf("Caracteres: %d\n", c);
  printf("Linhas:     %d\n", l);

  return 0;
}

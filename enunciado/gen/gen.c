#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#define PP_NARG(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14,  \
                 _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26,   \
                 _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38,   \
                 _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,   \
                 _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,   \
                 _63, N, ...)                                                  \
  N
#define PP_RSEQ_N()                                                            \
  63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,  \
      44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,  \
      26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,   \
      8, 7, 6, 5, 4, 3, 2, 1, 0

#define SCANF(fmt, ...)                                                        \
  if (scanf(fmt, __VA_ARGS__) != PP_NARG(__VA_ARGS__)) {                       \
    printf("Read error at line number %d in %s(%s)\n", __LINE__, __func__,     \
           __FILE__);                                                          \
    exit(1);                                                                   \
  }

#define FSCANF(file, fmt, ...)                                                 \
  if (fscanf(file, fmt, __VA_ARGS__) != PP_NARG(__VA_ARGS__)) {                \
    printf("Read error at line number %d in %s(%s)\n", __LINE__, __func__,     \
           __FILE__);                                                          \
    exit(1);                                                                   \
  }

#define I64(x) ((int64_t) x)

#define CHECKINT32(x)                                                          \
  if (x > INT32_MAX || x < INT32_MIN) {                                        \
    printf("Limit 32! %" PRId64 " at line number %d in %s(%s)\n", x, __LINE__, \
           __func__, __FILE__);                                                \
    exit(1);                                                                   \
  }

#define CHECKINT(x, blimit, tlimit)                                            \
  CHECKINT32(x);                                                               \
  if (x < blimit || x > tlimit) {                                              \
    printf("Limit! %" PRId64 " [%" PRId64 ",%" PRId64 "] at line number %d in %s(%s)\n", x,    \
           blimit, tlimit, __LINE__, __func__, __FILE__);                      \
    exit(1);                                                                   \
  }

#define NOTNULL(x)                                                             \
  if (!x) {                                                                    \
    printf("Null pointer at line number %d in %s(%s)\n", __LINE__, __func__,   \
           __FILE__);                                                          \
    exit(1);                                                                   \
  }

/******************************************************************************/

#define BLIMIT I64(0)
#define TLIMIT I64(100000)
#define CHECKINTLIMIT(x) CHECKINT(x, BLIMIT, TLIMIT)

/******************************************************************************/

int main(int argc, char *argv[]) {

  if (argc != 4) {
    printf("Usage: ./gen <pre> <input> <response>\n");
    exit(1);
  }

  FILE *pre = fopen(argv[1], "r");
  NOTNULL(pre);

  FILE *input = fopen(argv[2], "w");
  NOTNULL(input);

  char v;
  int64_t l = 0;
  int64_t c = 0;

  while ((v = fgetc(pre)) != EOF) {
    if (v == '\n') {
      l++;
      CHECKINTLIMIT(l);
    } else if (v < ' ' || v > '~') {
      printf("Invalid char: %c %d\n", v, (int)v);
      exit(1);
    } else {
      c++;
      CHECKINTLIMIT(c);
    }
    fputc(v, input);
  }

  fclose(input);
  fclose(pre);

  // Response
  FILE *response = fopen(argv[3], "w");
  NOTNULL(response);

  fprintf(response, "Caracteres: %" PRId64 "\n", c);
  fprintf(response, "Linhas:     %" PRId64 "\n", l);

  fclose(response);

  return 0;
}

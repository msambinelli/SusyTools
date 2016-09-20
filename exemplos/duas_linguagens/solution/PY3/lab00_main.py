#!/usr/bin/env python3

import lab00

import sys

l = 0
c = 0

while __name__ == '__main__':
    v = sys.stdin.read(1)
    if not v:
        break
    elif lab00.isBreakLine(v):
        l += 1
    else:
        c += 1

print("Caracteres: {0:d}".format(c));
print("Linhas:     {0:d}".format(l));

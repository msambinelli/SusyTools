#!/usr/bin/env python3

import sys

if len(sys.argv) != 5:
    print("Usage: ./gen <pre> <input> <output> <err>\n")
    sys.exit(1)

P = open(sys.argv[1], "r");
I = open(sys.argv[2], "w");
R = open(sys.argv[3], "w");
E = open(sys.argv[4], "w");

l=0
c=0
for line in P:
    I.write(line)
    l+=1
    c+=len(line)
    if line[-1] == "\n":
        c-=1

P.close()
I.close()

R.write("Caracteres: {}\n".format(c))
R.write("Linhas:     {}\n".format(l))
R.close()

E.close()

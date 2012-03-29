param low :  int(32) = -1;
param high: uint(32) = 1;

for param i in low..high {
  writeln((i, typeToString(i.type)));
}

for i in low..high {
  writeln((i, typeToString(i.type)));
}

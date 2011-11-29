param low : int(32) = 2147483647;
param high: int(64) = 2147483648;

for param i in low..high {
  writeln((i, typeToString(i.type)));
}

for i in low..high {
  writeln((i, typeToString(i.type)));
}

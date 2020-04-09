proc f(s: string ...?n, i: int = 1) {
  writeln("s = ", s);
  writeln("n = ", n);
  writeln("i = ", i);
}

f("one", "two", "three", 4);
f("one", "two", "three");

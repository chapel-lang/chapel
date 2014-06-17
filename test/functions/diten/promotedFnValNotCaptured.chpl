proc f(i: int) {
  writeln("f(", i, ")");
  return i;
}

proc main {
  f(1..5);
}


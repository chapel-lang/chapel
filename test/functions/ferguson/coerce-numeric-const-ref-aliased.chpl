var global: int(16) = 1;

f(global);

proc f(const ref g: int) {
  global = 5;
  writeln(g);
}

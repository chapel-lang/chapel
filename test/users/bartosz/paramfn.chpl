proc f(param x) param
  return x;

proc main() {
    param x = f("foo");
    writeln(x);
}

proc f(param x) param do
  return x;

proc main() {
    param x = f("foo");
    writeln(x);
}

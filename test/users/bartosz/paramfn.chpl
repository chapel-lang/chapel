proc f(param x: string) param
  return x;

proc main() {
    param x = f("foo");
    writeln(x);
}

proc f(param x: string) param : string
  return x;

proc main() {
    param x = f("foo");
    writeln(x);
}

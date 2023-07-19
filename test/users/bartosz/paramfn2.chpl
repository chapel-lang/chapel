proc f(param x: string) param : string do
  return x;

proc main() {
    param x = f("foo");
    writeln(x);
}

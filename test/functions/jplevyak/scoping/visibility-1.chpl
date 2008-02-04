
record foo {
  def f() { return 1; }
}
def f(x : foo) { return 2; }

{
  def f(x : foo) { return 3; }
  {
    def f(x : foo) { return 4; }
  }
  var x = new foo();
  writeln(x.f());
  writeln(f(x));
}

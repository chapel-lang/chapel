record bar {
  var x : int;
}
record foo : bar {
  var y : int;
}
def f(x : foo) { return 2; }

{
  def f(x : bar) { return 3; }
  {
    def f(x : foo) { return 4; }
  }
  var x = new foo();
  writeln(f(x));
}

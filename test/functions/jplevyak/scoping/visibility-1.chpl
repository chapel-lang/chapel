
record foo {
  proc f() { return 1; }
}
proc f(x : foo) { return 2; }

{
  proc f(x : foo) { return 3; }
  {
    proc f(x : foo) { return 4; }
  }
  var x = new foo();
  writeln(x.f());
  writeln(f(x));
}

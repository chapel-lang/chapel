record bar {
  var x : int;
}
record foo : bar {
  var y : int;
}
proc f(x : foo) { return 2; }

{
  proc f(x : bar) { return 3; }
  {
    proc f(x : foo) { return 4; }
  }
  var x = new foo();
  writeln(f(x));
}

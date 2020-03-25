class pair {
  var x: int;
  var y: real;
}

proc main() {
  var a: unmanaged pair?;

  a   = new unmanaged pair();
  a!.x = 10;
  a!.y = 2.3;

  writeln("a is: ", a!.x, " ", a!.y);

  delete a;
}

class A {
  var x;
}

{
  var z: shared A? = new shared A(7);
  var x: shared A?;
  x = new shared A(8);
  x = z;
  writeln(z!.x);
  writeln(x!.x);
}

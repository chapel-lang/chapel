class A {
  var x;
}

{
  var z: owned A? = new A(7);
  var x: owned A?;
  x = new A(8);
  x = z;
  writeln(x!.x);
}

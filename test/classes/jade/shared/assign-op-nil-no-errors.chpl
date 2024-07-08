class A {
  var x: int;
}

{
  var s: shared A = new shared A(8);
  s = nil;
}

{
  var s: shared A? = new shared A(9);
  s = nil;
}

{
  var s1: shared A = new shared A(10);
  var s2 = s1;
  s1 = nil;
  writeln(s2.x); // not an error
}

class A {
  var x;
}

proc doit(type C)
{
  var c = new C(1);
  writeln(c);
  writeln(c.x);
}

doit(A);


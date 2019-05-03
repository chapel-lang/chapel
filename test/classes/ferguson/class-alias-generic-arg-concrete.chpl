class A {
  type T;
  var x : T;
}

proc doit(type C)
{
  var c = new C(1);
  writeln(c);
  writeln(c.x);
  delete c;
}

doit(unmanaged A(int));

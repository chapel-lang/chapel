class A {
  type T;
  var x : T;
}

proc doit(type C)
{
  var c = new unmanaged C(1);
  writeln(c);
  writeln(c.x);
  delete c;
}

proc doit2(type C)
{
  var c = new owned C(2);
  writeln(c);
  writeln(c.x);
}
  
doit(unmanaged A(int));
doit(owned A(int));
doit(shared A(int));
doit(borrowed A(int));

doit2(unmanaged A(int));
doit2(owned A(int));
doit2(shared A(int));
doit2(borrowed A(int));

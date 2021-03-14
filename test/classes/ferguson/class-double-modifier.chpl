class A {
  type T;
  var x : T;
}

proc doit(type C)
{
  var c = new unmanaged C(1);
  writeln(c);
  writeln(c.x);
  writeln(c.type:string);
  delete c;
}

proc doit2(type C)
{
  var c = new owned C(2);
  writeln(c);
  writeln(c.x);
  writeln(c.type:string);
}

proc doit3(type C) {
  var c = new shared C(3);
  writeln(c);
  writeln(c.x);
  writeln(c.type:string);
}
  
proc doit4(type C)
{
  var c = new C(4);
  writeln(c);
  writeln(c.x);
  writeln(c.type:string);
  if (isUnmanagedClassType(C)) then
    delete c;
}

writeln("Section 1");
doit(unmanaged A(int));
doit(owned A(int));
doit(shared A(int));
doit(borrowed A(int));
doit(A(int));

writeln("Section 2");
doit2(unmanaged A(int));
doit2(owned A(int));
doit2(shared A(int));
doit2(borrowed A(int));
doit2(A(int));

writeln("Section 3");
doit3(unmanaged A(int));
doit3(owned A(int));
doit3(shared A(int));
doit3(borrowed A(int));
doit3(A(int));

writeln("Section 4");
doit4(unmanaged A(int));
doit4(owned A(int));
doit4(shared A(int));
doit4(borrowed A(int));
doit4(A(int));

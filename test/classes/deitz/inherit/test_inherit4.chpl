class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

proc foo(c : unmanaged C?) {
  writeln(c);
}

var c : unmanaged C?;

c = new unmanaged C();
foo(c);
delete c;
c = new unmanaged D();
foo(c);
delete c;

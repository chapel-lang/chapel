class C {
  var x : int = 2;
  proc foo(y : int = 4) {
    writeln(y, " and ", x);
  }
}

var c = new unmanaged C();
writeln(c);
c.foo(3);
c.foo();
delete c;

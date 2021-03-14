class C {
  var x : int;
  proc foo(i : int) {
    x = i;
  }
}

var c = new unmanaged C();
c.foo(i=2);
writeln(c);
delete c;

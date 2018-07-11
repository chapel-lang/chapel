class C {
  var x : int;
  var y : int;
  proc foo(i : int, j : int) {
    x = i;
    y = j;
  }
}

var c = new unmanaged C();
c.foo(j=3,i=2);
writeln(c);
delete c;

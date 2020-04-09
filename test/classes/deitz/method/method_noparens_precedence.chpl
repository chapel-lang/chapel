class C {
  var y: 3*int;
  proc x return y;
  proc x(i: int) return 4;
}

var c = new unmanaged C();
c.y = (1, 2, 3);
writeln(c);
writeln(c.x);
writeln(c.x(2));
delete c;

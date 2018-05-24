class C {
  var x: int;
}

var c1 = new unmanaged C(2);
var c2 = new unmanaged C(4);

writeln(c1, " ", c2);
c1.x = c2.x;
writeln(c1, " ", c2);

delete c1;
delete c2;

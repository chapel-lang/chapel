class C {
  var x: int;
}

var c = new unmanaged C();
c.x = 2;
var cx = c.x;
writeln(cx);
delete c;

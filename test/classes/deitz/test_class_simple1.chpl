class C {
  var x: int;
}

var c = new C();
c.x = 2;
var cx = c.x;
writeln(cx);
delete c;

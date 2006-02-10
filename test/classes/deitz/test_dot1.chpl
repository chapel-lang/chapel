class C {
  var x : integer;
}

var c = C();
var d = C(x = 3);
c.x = d.x;
writeln(c);
writeln(d);

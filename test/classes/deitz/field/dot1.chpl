class C {
  var x : int;
}

var c = new C();
var d = new C(x = 3);
c.x = d.x;
writeln(c);
writeln(d);
delete c;
delete d;

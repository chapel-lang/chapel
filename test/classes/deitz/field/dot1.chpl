class C {
  var x : int;
}

var c = new unmanaged C();
var d = new unmanaged C(x = 3);
c.x = d.x;
writeln(c);
writeln(d);
delete c;
delete d;

class C {
  type t;
  var x : t;
  var y;
}

var c = new unmanaged C(t=int, y=12.0);
writeln(c);
c.y = 14.0;
writeln(c);
delete c;

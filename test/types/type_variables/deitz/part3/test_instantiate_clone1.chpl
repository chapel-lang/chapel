class C {
  type t;
  var x : t;
  var y;
}

var c = new unmanaged C(int, y=12.0);

writeln(c);

c.x = 12;

writeln(c);

delete c;

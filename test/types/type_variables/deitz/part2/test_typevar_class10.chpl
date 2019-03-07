class bar {
  type t;
  var y : t;
  proc clone()
    return new unmanaged bar(t, y);
}

var b : unmanaged bar(int) = new unmanaged bar(int, 4);

writeln(b);

var c = b.clone();

writeln(c);

c.y = 12;

writeln(b);
writeln(c);

delete c;
delete b;


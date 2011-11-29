class bar {
  type t;
  var y : t;
  proc clone()
    return new bar(t, y);
}

var b : bar(int) = new bar(int, 4);

writeln(b);

var c = b.clone();

writeln(c);

c.y = 12;

writeln(b);
writeln(c);

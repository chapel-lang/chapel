class bar {
  type t;
  var y : t;
  fun clone()
    return bar(t, y);
}

var b : bar(int) = bar(int, 4);

writeln(b);

var c = b.clone();

writeln(c);

c.y = 12;

writeln(b);
writeln(c);

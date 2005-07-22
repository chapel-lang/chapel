class bar {
  type t;
  var y : t;
  function clone()
    return bar(t, y);
}

var b : bar(integer) = bar(integer, 4);

writeln(b);

var c = b.clone();

writeln(c);

c.y = 12;

writeln(b);
writeln(c);

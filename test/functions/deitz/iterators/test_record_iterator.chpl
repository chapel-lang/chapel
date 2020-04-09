record R {
  var x, y, z: int;
  iter iterator() {
    x = 1;
    yield x;
    y = 2;
    yield y;
    z = 3;
    yield z;
  }
}

var r: R;

writeln(r);
for i in r.iterator() do
  writeln(i);
writeln(r);

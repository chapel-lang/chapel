class EC {
  var i: sync int(64) = 0;
  var b: sync bool = true;
  // TODO: compiler-generated initializer
}

proc up(e: borrowed EC) {
  var i = e.i.readFE();
  if i == 0 then
    e.b.reset();
  e.i.writeEF(i + 1);
}

proc down(e: borrowed EC) {
  var i = e.i.readFE();
  if i == 1 then
    e.b.writeEF(true);
  e.i.writeEF(i - 1);
}

proc allocate() {
  return new unmanaged EC();
}

proc wait(e: unmanaged EC) {
  e.b.readFE();
  e.i.readFE(); // wait until down is finished setting i
  delete e;
}

config var n = 1000;

proc main {
  var a = 0;
  for i in 1..n {
    //var unused = allocate();
    var e = allocate();
    up(e);
    begin with (ref a) {
      a += 1;
      down(e);
    }
    wait(e);
  }
  writeln(a == n);
}

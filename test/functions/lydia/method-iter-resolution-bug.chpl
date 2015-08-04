class C {
  var start = 17;

  proc iTerate(x: int) {
    // Problem seems to stem from calling an outside iterator from a
    // method
    return iTerate(start, start+x);
  }
}

iter iTerate(start: int, end: int) {
  for i in start..end {
    yield i;
  }
}

var c = new C(17);
for i in c.iTerate(7) {
  writeln(i);
}

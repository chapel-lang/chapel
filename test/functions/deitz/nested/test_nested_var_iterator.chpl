proc foo() {
  var x, y, z: int;
  iter bar() ref {
    yield x;
    yield y;
    yield z;
  }
  var j = 1;
  for i in bar() {
    i = j;
    j += 1;
  }
  writeln((x, y, z));
}

foo();

iter myIterator() {
  var x = 1;
  defer {
    x = 2;
    writeln("in iterator defer ", x);
  }
  writeln("in iterator ", x);
  for i in 1..10 {
    yield i;
  }
}

proc test() {
  for i in myIterator() {
    writeln("got i ", i);
  }
}

test();

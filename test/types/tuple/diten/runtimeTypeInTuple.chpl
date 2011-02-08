proc aValue() {
  var val: (complex, [1..3].type, int);
  return val;
}

proc anyTypeWriteln(args...?n) {
  for param i in 1..n {
    write(args(i), " ");
  }
  writeln();
}

var a = aValue();
anyTypeWriteln((...a));

// Case #1: Generic
proc bar((x , y)) where isRealType(x.type) && isImagType(y.type) {
  return x * y;
}

// Case #2: Concrete
proc baz((x, y): (int, int)) where isIntType(x.type) && isIntType(y.type) {
  return x*y;
}

proc main() {
  var n1 = bar((2.0, 8.0i));
  writeln(n1);
  var n2 = baz((8, 8));
  writeln(n2);
}

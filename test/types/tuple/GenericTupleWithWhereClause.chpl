proc bar((x , y)) where isRealType(x.type) && isImagType(y.type) {
  return x * y;
}

proc main() {
  var n = bar((2.0, 8.0i));
  writeln(n);
}

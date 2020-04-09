config const n = 10;

proc main() {
  var a = 0;
  var b = 1;

  test(a, b);
  writeln(a);
}

proc test(ref a, ref b) {
  b += 0;
  for i in 1..n {
    a += b;
  }
}

config const n = 10;

proc main() {
  var a = 1;

  test(a, a);
  writeln(a);
}

proc test(ref a, ref b) {
  b += 0;
  for i in 1..n {
    a += b;
  }
}


var span = max(int)..min(int);  // degenerate range

iter genInts() {
  yield 2;
  yield 0;
  yield 1;
  yield -1;
}

writeln("span is: ", span);
for i in genInts() {
  if (i < span.low) {
    span = i..span.high;
  }
  if (i > span.high) {
    span = span.low..i;
  }
  writeln("span is: ", span);
}

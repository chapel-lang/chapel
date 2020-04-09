// from #9982

proc modify(const ref A) {
  (A[0],) = (1,); // A[0] = 1;
}

proc main() {
  var array: [0..#10] int;
  modify(array);
  writeln(array);
}

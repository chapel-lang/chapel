export proc foo(in x: [] int) {
  writeln(x); // Note: this assumes x will have initial contents
  for i in x.domain {
    x[i] = x[i] + 1;
  }
}

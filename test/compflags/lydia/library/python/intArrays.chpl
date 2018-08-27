export proc takesArray(x: [] int) {
  writeln(x); // Note: this assumes x will have initial contents
  for i in x.domain {
    x[i] = x[i] + 1;
  }
}

export proc returnsArray(): [0..3] int {
  var x: [0..3] int = [1, 2, 3, 4];
  return x;
}

export proc takesArray(in x: [] real) {
  for i in x.domain {
    // Note: this assumes x will have initial contents
    if ((x[i] <= i+2) && (x[i] > i)) {
      writeln("x[", i, "] in expected range");
    }
  }
}

export proc returnsArray(): [0..3] real {
  var x: [0..3] real = [1.0, 2.0, 3.0, 4.0];
  return x;
}

export proc foo(ref x: [] real) {
  for i in x.domain {
    // Note: this assumes x will have initial contents
    if ((x[i] <= i+2) && (x[i] > i)) {
      writeln("x[" + i:string + "] in expected range");
    }
  }
  for i in x.domain {
    x[i] = x[i] + 1;
  }
}

export proc foo(ref x: [] int, const ref y: [] int) {
  writeln(x); // Note: this assumes x will have initial contents
  writeln(y); // Note: this assumes y will have initial contents
  if (x.size == y.size) {
    for i in x.domain {
      x[i] = x[i] + y[i];
    }
  } else {
    var minimum = min(x.size, y.size);
    for i in 0..#minimum {
      x[i] = x[i] + y[i];
    }
  }
}

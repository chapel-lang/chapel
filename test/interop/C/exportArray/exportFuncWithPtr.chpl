export proc foo(ref x: [] int, otherVal: c_ptr(int)) {
  writeln(x); // Note: this assumes x will have initial contents
  for i in x.domain {
    x[i] = x[i] + 1;
  }
  writeln(otherVal.deref());
}

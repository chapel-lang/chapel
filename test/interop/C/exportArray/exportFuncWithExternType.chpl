export proc foo(ref x: [] int, const ref otherVal: chpl_external_array) {
  // Note: this assumes the type otherVal stores
  var explicitlyWrapped = makeArrayFromExternArray(otherVal, int);
  writeln(x); // Note: this assumes x will have initial contents
  writeln(explicitlyWrapped); // Note: this assumes y will have initial contents
  for i in x.domain {
    x[i] = x[i] + 1;
  }
}

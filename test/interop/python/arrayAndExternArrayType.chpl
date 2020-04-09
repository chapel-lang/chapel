export proc takesArrays(x: [] int, otherVal: chpl_external_array) {
  // Note: this assumes the type otherVal stores
  var explicitlyWrapped = makeArrayFromExternArray(otherVal, int);
  writeln(x); // Note: this assumes x will have initial contents
  writeln(explicitlyWrapped); // Note: this assumes y will have initial contents
  for i in x.domain {
    x[i] = x[i] + 1;
  }
}

export proc returnsExternArray(): chpl_external_array {
  var x: [0..3] int = [1, 2, 3, 4];
  var retval = convertToExternalArray(x);
  return retval;
}

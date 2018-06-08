use ExternalArray;

export proc foo(): chpl_external_array {
  var x = getArray();
  writeln(x);
  var retval = makeReturnableArr(x);
  return retval;
}

proc getArray(): [0..3] int {
  var x: [0..3] int = [1, 2, 3, 4];
  return x;
}
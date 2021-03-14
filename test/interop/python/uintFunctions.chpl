/* Tests accepting a uint */
export proc takesUint(x: uint) {
  writeln(x);
}

/* Tests returning a uint */
export proc getUint(): uint {
  var ret: uint = 4;
  return ret;
}

/* Tests taking and returning a uint */
export proc takeAndReturn(x: uint): uint {
  return x;
}

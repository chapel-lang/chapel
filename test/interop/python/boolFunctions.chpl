/* Tests accepting a bool */
export proc takesBool(x: bool) {
  writeln(x);
}

/* Tests returning a bool */
export proc getBool(): bool {
  var ret: bool = true;
  return ret;
}

/* Tests taking and returning a bool */
export proc takeAndReturn(x: bool): bool {
  return x;
}

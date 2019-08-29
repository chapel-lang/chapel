// Chapel file that exports functions: one that takes an real, one that returns
// an real, and one that does both.
export proc take(x: real) {
  var xFloor = floor(x);
  var xCeiling = ceil(x);
  writeln("Was given x in range of: ", xFloor, " and ", xCeiling);
}

export proc give(): real {
  return 5.3;
}

export proc takeAndReturn(x: real): real {
  return x;
}

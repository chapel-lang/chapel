class MyError : Error {
  var x: int;
}

proc main() {
  // Use after free.
  var ex = new owned MyError(1);
  try {
    throw ex;
  } catch e {
    // intentionally empty
  }

  writeln(ex.x);
}

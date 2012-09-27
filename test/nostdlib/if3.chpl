proc main() {
  var sum = 0;
  if sum == 0 {
    __primitive("chpl_error", "OK");
  } else {
    __primitive("chpl_error", "BAD");
  }
}



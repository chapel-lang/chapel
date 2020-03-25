/*
  verifies that only those functions are counted which
  takes an `int` type as an argument. This is because an `int`
  is passed as an argument to the primitive.
*/
proc dummy_test_int(test: int) throws {
    //This is a dummy test that takes int
}

proc dummy_test_real(test: string) throws {
    //This is a dummy test that takes string
}

var test = 5;
writeln(__primitive("gather tests", (test)));

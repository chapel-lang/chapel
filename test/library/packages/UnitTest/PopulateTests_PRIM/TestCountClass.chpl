use UnitTest;
class dummy {
  /* A method that takes Test object as an argument*/
  proc test_class(test: borrowed Test) throws {
    // Test inside a class
  }
}

var test: owned Test = new owned Test();
writeln(__primitive("gather tests", test.borrow()));

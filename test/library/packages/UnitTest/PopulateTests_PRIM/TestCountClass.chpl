use UnitTest;
class dummy {
  /* A method that takes Test object as an argument*/
  proc test_class(test: Test) throws {
    // Test inside a class 
  }
}

var test = new Test();
writeln(__primitive("gather tests", (test)));

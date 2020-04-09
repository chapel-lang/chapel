use UnitTest;
//verifies that functions with multiple argument aren't counted

/* A normal function which takes a multiple argument*/
proc dummy_test(test: borrowed Test, test2) throws {
    //This is a dummy test
}

/* A normal function which takes a multiple argument*/
proc dummy_test_default(test: borrowed Test, test2 = 5) throws {
    //This is a dummy test
}

// Test with varargs
proc dummy_test_varargs(test: borrowed Test ...?n) {
    //this is var arg test
}

var test: owned Test = new owned Test();
writeln(__primitive("gather tests", test.borrow()));

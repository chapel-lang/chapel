use UnitTest;
//verifies that functions with multiple argument aren't counted

/* A normal function which takes a multiple argument*/
proc dummy_test(test: Test, test2) throws {
    //This is a dummy test
}

/* A normal function which takes a multiple argument*/
proc dummy_test_default(test: Test, test2 = 5) throws {
    //This is a dummy test
}

// Test with varargs
proc dummy_test_varargs(test: Test ...?n) {
    //this is var arg test
}

var test = new Test();
writeln(__primitive("gather tests", (test)));

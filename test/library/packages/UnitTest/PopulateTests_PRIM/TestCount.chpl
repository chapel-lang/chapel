use UnitTest;
//verifies that generic single argument functions aren't counted

/* A normal function which takes a generic single argument*/
proc dummy_test(test) throws {
    //This is a dummy test
}

/* A test function which takes Test Object as argument*/
proc test_square(test: Test) throws {
    // This is a test function
}
var test = new Test();
writeln(__primitive("gather tests", (test)));

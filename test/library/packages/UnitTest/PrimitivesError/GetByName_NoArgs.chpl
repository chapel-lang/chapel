use UnitTest;

/* A test function which takes Test Object as argument*/
proc test_square(test: borrowed Test) throws {
    // This is a test function
    writeln("b");
}

/* A test function which takes Test Object as argument*/
proc test_squared(test: borrowed Test) throws {
    // This is a test function
    writeln("c");
}


var test: owned Test = new owned Test();
param a = __primitive("gather tests",test.borrow());
var c = __primitive("get test by name");

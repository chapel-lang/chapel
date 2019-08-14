use UnitTest;
//verifies that generic single argument functions aren't counted

/* A normal function which takes a generic single argument*/
proc dummy_test(test) throws {
    //This is a dummy test
    writeln("a");
}

/* A test function which takes Test Object as argument*/
proc test_square(test: Test) throws {
    // This is a test function
    writeln("b");
}
/* A test function which takes Test Object as argument*/
proc test_squared(test: Test) throws {
    // This is a test function
    writeln("c");
}


var test = new Test();
param a = __primitive("gather tests", (test));
var c = __primitive("get test by name","test_square");
var d = __primitive("get test by index",2);
c(test);
d(test);
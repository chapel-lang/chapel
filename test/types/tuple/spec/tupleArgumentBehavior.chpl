//
// This test will verify what the spec has to say about different argument
// intents for tuples. May have to be an overloaded test using param
// constants.
//

record R { var x: int = 0; }

proc test1(tup: (int, R)) {

}

proc test2(ref tup: (int, R)) {

}

proc test3(const tup: (int, R)) {

}

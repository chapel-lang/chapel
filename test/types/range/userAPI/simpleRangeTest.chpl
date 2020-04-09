use rangeAPItest;

// not exhaustive, but test some simple cases
testRangeAPI("basic range", 1..10, 4, 3..6);
testRangeAPI("strided range", 1..20 by 2, 7, 5..13 by 2);
testRangeAPI("neg. str. range", 1..20 by -2, 14, 16..8 by -2);
testRangeAPI("unbounded range", 1.., 4, 3..6);
testRangeAPI("upper unbnd range", ..10, 4, 3..6);
testRangeAPI("neg upper unbnd range", ..10 by -1, 4, 3..6 by -1);
testRangeAPI("unbounded range", .., 4, 3..6);

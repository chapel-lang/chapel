use rangeAPItest;

enum color { red };
use color;

testRangeAPI("trivial enum range", red..red, red, red..red, 0, 1);

var r: range(color);
testRangeAPI("default trivial enum range", r, red, r, 0, 0);

testRangeAPI("trivial strided enum range", red..red by 1, red, red..red, 0, 1);

var r2: range(color, stridable=true);
testRangeAPI("default trivial strided enum range", r2, red, r, 0, 0);


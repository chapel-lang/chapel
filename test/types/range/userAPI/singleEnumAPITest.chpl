use rangeAPItest;

enum color { red };
use color;

testRangeAPI("trivial enum range", red..red, red, red..red, 0, 1);

var r: range(color);
testRangeAPI("default trivial enum range", r, red, r, 0, 0);

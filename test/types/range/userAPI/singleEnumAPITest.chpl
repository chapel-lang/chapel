use rangeAPItest;

enum color { red };
use color;
config const dummyStride = 1;

testRangeAPI("trivial enum range", red..red, red, red..red, 0, 1);

var r: range(color);
testRangeAPI("default trivial enum range", r, red, r, 0, 0);

testRangeAPI("trivial strided enum range", red..red by dummyStride, red, red..red, 0, 1);

var r2: range(color, strides=strideKind.any);
testRangeAPI("default trivial strided enum range", r2, red, r, 0, 0);


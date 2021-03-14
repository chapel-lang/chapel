use driver;
config const n = 1024;

// declare a few arithmetic arrays for verification purposes
const D = {1..n};

var ArithInt, ArithIntRef: [D] intType;
var ArithUint, ArithUintRef: [D] uintType;
var ArithReal, ArithRealRef: [D] realType;
var ArithString, ArithStringRef: [D] stringType;


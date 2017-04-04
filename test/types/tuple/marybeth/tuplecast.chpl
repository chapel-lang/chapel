// This test requests that cast to a tuple type
// replicates a value into a tuple (here, 3.14 into
// a 10-tuple of 3.14s). This was not desirable
// as per 2adae43, however we may want to revisit.

var x: real;
var y: 10*real;

x = 3.14;
y = x: (10*real);

writeln(y);

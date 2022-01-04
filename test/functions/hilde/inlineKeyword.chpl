// Test that inline works as a keyword.
inline operator real.+(a: real, b: real) return 10.0;

var y = 1.0;
var x = y + 2.0;

writeln(x);


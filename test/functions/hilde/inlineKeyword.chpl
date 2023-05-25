// Test that inline works as a keyword.
inline operator +(a: real, b: real) do return 10.0;

var y = 1.0;
var x = y + 2.0;

writeln(x);


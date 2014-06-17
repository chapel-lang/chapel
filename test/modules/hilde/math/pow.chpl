// Demonstrate the pow() math function.
var x = 2.0;
var y = 1.5;

// Compute something close to 2 * sqrt(2).
var z = pow(x, y);

writeln("2 to the 1 1/2 power is ", z);
writeln(abs(z - x**y) < 1.0e-9);	// Should be "true".
writeln(abs(z - 2.0 * sqrt(2.0)) < 1.0e-9);    // "true"


use Math;

writeln(isClose(halfPi, pi/2));     // Should trigger for halfPi
writeln(isClose(quarterPi, pi/4));  // Should trigger for quarterPi
writeln(isClose(reciprPi, 1/pi));   // Should trigger for reciprPi
// Should trigger for twiceReciprPi
writeln(isClose(twiceReciprPi, 2/pi));
// Should trigger for twiceReciprSqrtPi
writeln(isClose(twiceReciprSqrtPi, 2/sqrt(pi)));
writeln(isClose(sqrt2, sqrt(2)));   // Should trigger for sqrt2
writeln(isClose(reciprSqrt2, 1/sqrt(2))); // Should trigger for reciprSqrt2

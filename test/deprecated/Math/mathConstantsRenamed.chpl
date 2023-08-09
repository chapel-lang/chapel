use Math;

writeln(isClose(log2_e, log2(e))); // Should trigger for log2_e
writeln(isClose(log10_e, log10(e))); // Should trigger for log10_e
writeln(isClose(ln_2, log(2)));      // Should trigger for ln_2
writeln(isClose(ln_10, log(10)));    // Should trigger for ln_10
writeln(isClose(half_pi, pi/2));     // Should trigger for half_pi
writeln(isClose(quarter_pi, pi/4));  // Should trigger for quarter_pi
writeln(isClose(recipr_pi, 1/pi));   // Should trigger for recipr_pi
// Should trigger for twice_recipr_pi
writeln(isClose(twice_recipr_pi, 2/pi));
// Should trigger for twice_recipr_sqrt_pi
writeln(isClose(twice_recipr_sqrt_pi, 2/sqrt(pi)));
writeln(isClose(sqrt_2, sqrt(2)));   // Should trigger for sqrt_2
writeln(isClose(recipr_sqrt_2, 1/sqrt(2))); // Should trigger for recipr_sqrt_2

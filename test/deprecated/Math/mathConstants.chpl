writeln(isclose(e, exp(1)));       // Should trigger for e
writeln(isclose(log2_e, log2(e))); // Should trigger for log2_e and e
writeln(isclose(log10_e, log10(e))); // Should trigger for log10_e and e
writeln(isclose(ln_2, log(2)));      // Should trigger for ln_2
writeln(isclose(ln_10, log(10)));    // Should trigger for ln_10
writeln(isclose(pi, 3.141592653));   // Should trigger for pi
writeln(isclose(half_pi, pi/2));     // Should trigger for half_pi, pi
writeln(isclose(quarter_pi, pi/4));  // Should trigger for quarter_pi, pi
writeln(isclose(recipr_pi, 1/pi));   // Should trigger for recipr_pi, pi
// Should trigger for twice_recipr_pi, pi
writeln(isclose(twice_recipr_pi, 2/pi));
// Should trigger for twice_recipr_sqrt_pi, pi
writeln(isclose(twice_recipr_sqrt_pi, 2/sqrt(pi)));
writeln(isclose(sqrt_2, sqrt(2)));   // Should trigger for sqrt_2
writeln(isclose(recipr_sqrt_2, 1/sqrt(2))); // Should trigger for recipr_sqrt_2

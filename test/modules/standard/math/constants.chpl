use Math;

proc check(x, y)
{
  param threshold = 0.000000000000001;
  if abs(x-y) < threshold {
    // OK!
  } else {
    writeln("comparison failed between ",
            x, " and ", y);
  }
}

check(e, exp(1));
check(log2_e, log2(e));
check(log10_e, log10(e));
check(ln_2, log(2));
check(ln_10, log(10));
assert(pi:int == 3);
check(sin(pi), 0.0);
check(half_pi, pi/2.0);
check(quarter_pi, pi/4.0);
check(recipr_pi, 1.0/pi);
check(twice_recipr_pi, 2.0/pi);
check(twice_recipr_sqrt_pi, 2.0/sqrt(pi));
check(sqrt_2, sqrt(2));
check(recipr_sqrt_2, 1/sqrt(2));


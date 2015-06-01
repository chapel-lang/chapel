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
check(log2e, log2(e));
check(log10e, log10(e));
check(ln_2, log(2));
check(ln_10, log(10));
assert(pi:int == 3);
check(sin(pi), 0.0);
check(pi2, pi/2.0);
check(pi4, pi/4.0);
//check(1_pi, 1.0/pi);
//check(2_pi, 2.0/pi);
//check(2_sqrtpi, 2.0/sqrt(pi));
check(sqrt2, sqrt(2));
check(sqrt1_2, 1/sqrt(2));

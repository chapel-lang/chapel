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
check(log2E, log2(e));
check(log10E, log10(e));
check(ln2, log(2));
check(ln10, log(10));
assert(pi:int == 3);
check(sin(pi), 0.0);
check(halfPi, pi/2.0);
check(quarterPi, pi/4.0);
check(reciprPi, 1.0/pi);
check(twiceReciprPi, 2.0/pi);
check(twiceReciprSqrtPi, 2.0/sqrt(pi));
check(sqrt2, sqrt(2));
check(reciprSqrt2, 1/sqrt(2));


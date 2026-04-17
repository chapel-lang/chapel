use BigInteger;

var x, a, b, c: bigint;
proc initVars() {
  a = 12345: bigint;
  b = 67890: bigint;
  c = 123456789: bigint;
}

initVars();
/*START_EXAMPLE_FAST*/
x  = b;
x *= c;
x += a;
/*STOP_EXAMPLE_FAST*/
writeln(x);

initVars();
/*START_EXAMPLE_SLOW*/
x = a + b * c;
/*STOP_EXAMPLE_SLOW*/
writeln(x);

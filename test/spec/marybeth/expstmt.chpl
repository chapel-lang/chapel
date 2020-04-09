var denom = 1.0;
var x: real;

testForZero(denom);
testForZero(x);
testForZero(0.0);

proc testForZero(x: real) {
  if (x == 0.0) then halt("Value is zero.");
  else writeln("Non-zero value.  Continuing.");   
}

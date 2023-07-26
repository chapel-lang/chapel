// deprecated in 1.32 by Jade
use BigInteger;


var numer: bigint = 10;
var denom: bigint = 3;
var res1, res2: bigint;

divQ(res1, numer, denom);
writeln("divQ ", res1);
divQ(res1, numer, 4);
writeln("divQ ", res1);
// trigger a version with round to make sure we get the extra deprecation
divQ(res1, numer, 4, BigInteger.round.up);
writeln("divQ ", res1);

divR(res1, numer, denom);
writeln("divR ", res1);
divR(res1, numer, 4);
writeln("divR ", res1);
// trigger a version with round to make sure we get the extra deprecation
divR(res1, numer, 4, BigInteger.round.up);
writeln("divR ", res1);

divQR(res1, res2, numer, denom);
writeln("divQR ", (res1, res2));
divQR(res1, res2, numer, 4);
writeln("divQR ", (res1, res2));
// trigger a version with round to make sure we get the extra deprecation
divQR(res1, res2, numer, 4, BigInteger.round.up);
writeln("divQR ", (res1, res2));
{ var denom:bigint = 5; // special so we have no remainder
  divexact(res1, numer, denom);
  writeln("divexact ", res1);
  divexact(res1, numer, 2);
  writeln("divexact ", res1);
}
divQ2Exp(res1, numer, 2);
writeln("divQ2Exp ", res1);
// trigger a version with round to make sure we get the extra deprecation
divQ2Exp(res1, numer, 2, BigInteger.round.up);
writeln("divQ2Exp ", res1);

divR2Exp(res1, numer, 2);
writeln("divR2Exp ", res1);
// trigger a version with round to make sure we get the extra deprecation
divR2Exp(res1, numer, 2,  BigInteger.round.up);
writeln("divR2Exp ", res1);

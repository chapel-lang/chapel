// deprecated in 1.32 by Jade

use BigInteger;


{
  compilerWarning("mod");
  writeln("mod");
  var a: bigint = 15;
  var b: bigint = 6;
  var res: bigint;
  mod(res, a=a, b=b); // warn
  writeln(res);
  mod(res, a=a, b=3); // warn
  writeln(res);
  mod(res, x=a, y=b);
  writeln(res);
  mod(res, x=a, y=-4);
  writeln(res);
  mod(res, a, b);
  writeln(res);
  mod(res, a, 2);
  writeln(res);
}

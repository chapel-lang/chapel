config var testnum = 1;
use BigInteger;

if(testnum == 1) {
  var a: bigint = 15;
  var b: int = 0;
  var res: bigint = a % b;
}
else if(testnum == 2) {
  var a: bigint = 15;
  var b: bigint = 0;
  var res: bigint = a % b;
}
else if(testnum == 3) {
  var a: bigint = 15;
  var b: uint = 0;
  var res: bigint;
  mod(res, a, b);
}
else if(testnum == 4) {
  var a: bigint = 15;
  var b: bigint = 0;
  var res: bigint;
  mod(res, a, b);
}
else if(testnum == 5) {
  var a: bigint = 15;
  var b: uint = 0;
  a %= b;
}
else if(testnum == 6) {
  var a: bigint = 15;
  var b: bigint = 0;
  a %= b;
}

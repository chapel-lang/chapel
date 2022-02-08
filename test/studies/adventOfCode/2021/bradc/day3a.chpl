use IO;

config const debug = false;

iter readBinaryStrings() {
  do {
    var i: int;
    const success = readf("%bi\n", i);
    if success then
      yield i;
  } while success;
}

var A = readBinaryStrings();
if debug then
  writeln(A);

var digits = 1 + max reduce log2(A);

if debug then
  writeln(digits);


var B = A;
var mask = 0x1 << (digits-1);

const O2gen = O2filter(mask, A, O2=true);
const CO2scrub = O2filter(mask, B, O2=false);
writeln(O2gen * CO2scrub);

proc O2filter(in mask, A, O2: bool): int {
  if debug then
    writeln("In O2filter: ", A);
  const   ones = + reduce ((A & mask) != 0);
  const zeroes = + reduce (((A & mask) ^ mask) != 0);
  var B = filter(A, mask, (ones >= zeroes) == O2);
  if (B.size == 1) then
    return B[0];
  else
    return O2filter(mask >> 1, B, O2);
}

iter filter(A, mask, ones) {
  for a in A do
    if ones {
      if a & mask then
        yield a;
    } else {
      if (a & mask) ^ mask then
        yield a;
    }
}

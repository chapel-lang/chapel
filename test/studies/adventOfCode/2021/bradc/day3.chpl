use IO;

config const debug = false;

iter readBinaryStrings() {
  var i: int;
  while readf("%bi\n", i) {
    yield i;
  }
}

var A = readBinaryStrings();
if debug then
  writeln(A);

var mask = 0x1;
var gamma, epsilon = 0;
do {
  const   ones = + reduce ((A & mask) != 0);
  const zeroes = + reduce (((A & mask) ^ mask) != 0);

  if ones {
    if ones > zeroes then
      gamma |= mask;
    else
      epsilon |= mask;
  }
  
  mask <<= 1;
} while ones;

if debug then
  writeln((gamma, epsilon));

writeln(gamma*epsilon);

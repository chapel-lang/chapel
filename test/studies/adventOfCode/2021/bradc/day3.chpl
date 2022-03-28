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

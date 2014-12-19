inline proc _defaultOf(type t) where (isComplexType(t)) {
  writeln("I default initialized!");
  var ret:t = noinit;
  param floatwidth = numBits(t)/2;
  ret.re = 0.0:real(floatwidth);
  ret.im = 0.0:real(floatwidth);
  return ret;
}

var blah: complex = noinit; // Should not print out the message
blah = 3.4 + 5i;
writeln(blah);
var blah2: complex;         // Should print out the message
writeln(blah2);

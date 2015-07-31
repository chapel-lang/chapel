var x: real = 2.0;

// create a number big enough that squaring it will be infinity
while !isinf(x*x) do
  x *= x;
extern proc fabs(x: real(64)): real(64);
var im: imag = _r2i(x);
writeln("      im: ", im);
writeln(" abs(im): ", abs(im));
writeln(" abs(im).type: ", typeToString(abs(im).type));
writeln("fabs(_i2r(im)): ", fabs(_i2r(im)));
writeln(" abs(im:complex): ", abs(im:complex));

var x32: real(32) = 2.0:real(32);

while !isinf(x32*x32) do
  x32 *= x32;

extern proc fabsf(x: real(32)): real(32);
var im32: imag(32) = _r2i(x32);
writeln("      im32: ", im32);
writeln(" abs(im32): ", abs(im32));
writeln(" abs(im32).type: ", typeToString(abs(im32).type));
writeln("fabsf(_i2r(im32)): ", fabsf(_i2r(im32)));
writeln(" abs(im32:complex(64)): ", abs(im32:complex(64)));

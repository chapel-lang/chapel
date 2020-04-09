// Test 64-bit and 64-bit complex numbers.

writeln( "\ncomplex(64) numbers");
var a64: complex(64);
writeln( "a64 = ", a64);
var f32: real(64) = 5.6:real(64);

var b64:complex(64) = (1.2 + 3.4i):complex(64);
writeln( "b64 = ", b64);
a64 = (8.9i + f32):complex(64);
writeln( "a64 = ", a64);

writeln( "a64 + b64 = ", a64 + b64);
writeln( "a64 + 2.0 = ", a64 + 2.0);
writeln( "a64 + 2.0i = ", a64 + 2.0i);
writeln( "a64 - b64 = ", a64 - b64);
writeln( "a64 - 2.0 = ", a64 - 2.0);
writeln( "a64 - 2.0i = ", a64 - 2.0i);
writeln( "a64 * b64 = ", a64 * b64);
writeln( "a64 / b64 = ", a64 / b64);
writeln( "a64 / 2.0 = ", a64 / 2.0);
writeln( "conjg(a64) = ", conjg(a64));


writeln( "\ncomplex(128) numbers");
var a128:complex(128);
writeln( "a128 = ", a128);
var f64: real(64) = 5.6;

var b128:complex(128) = 1.2 + 3.4i;
writeln( "b128 = ", b128);
a128 = 8.9i + f64;
writeln( "a128 = ", a128);

writeln( "a128 + b128 = ", a128 + b128);
writeln( "a128 + 2.0 = ", a128 + 2.0);
writeln( "a128 + 2.0i = ", a128 + 2.0i);
writeln( "a128 - b128 = ", a128 - b128);
writeln( "a128 - 2.0 = ", a128 - 2.0);
writeln( "a128 - 2.0i = ", a128 - 2.0i);
writeln( "a128 * b128 = ", a128 * b128);
writeln( "a128 / b128 = ", a128 / b128);
writeln( "a128 / 2.0 = ", a128 / 2.0);
writeln( "conjg(a128) = ", conjg(a128));


writeln( "\ncomplex numbers");
var r128:complex;
writeln( "r128 = ", r128);
var t128: real = 5.6;

var s128:complex = 1.2 + 3.4i;
writeln( "s128 = ", s128);
r128 = 8.9i + t128;
writeln( "r128 = ", r128);

writeln( "r128 + s128 = ", r128 + s128);
writeln( "r128 + 2.0 = ", r128 + 2.0);
writeln( "r128 + 2.0i = ", r128 + 2.0i);
writeln( "r128 - s128 = ", r128 - s128);
writeln( "r128 - 2.0 = ", r128 - 2.0);
writeln( "r128 - 2.0i = ", r128 - 2.0i);
writeln( "r128 * s128 = ", r128 * s128);
writeln( "r128 / s128 = ", r128 / s128);
writeln( "r128 / 2.0 = ", r128 / 2.0);
writeln( "conjg(r128) = ", conjg(r128));


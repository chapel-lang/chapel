// Test 32-bit and 64-bit complex numbers.

writeln( "\ncomplex(32) numbers");
var a32: complex(32);
writeln( "a32 = ", a32);
a32 = _max(a32.type);
writeln( "max(a32) = ", a32);
var f32: float(32) = 5.6:float(32);

var b32:complex(32) = (1.2 + 3.4i):complex(32);
writeln( "b32 = ", b32);
a32 = (8.9i + f32):complex(32);
writeln( "a32 = ", a32);

writeln( "a32 + b32 = ", a32 + b32);
writeln( "a32 + 2.0 = ", a32 + 2.0);
writeln( "a32 + 2.0i = ", a32 + 2.0i);
writeln( "a32 - b32 = ", a32 - b32);
writeln( "a32 - 2.0 = ", a32 - 2.0);
writeln( "a32 - 2.0i = ", a32 - 2.0i);
writeln( "a32 * b32 = ", a32 * b32);
writeln( "a32 / b32 = ", a32 / b32);
writeln( "a32 / 2.0 = ", a32 / 2.0);
writeln( "conjg(a32) = ", conjg(a32));


writeln( "\ncomplex(64) numbers");
var a64:complex(64);
writeln( "a64 = ", a64);
a64 = _max(a64.type);
writeln( "max(a64) = ", a64);
var f64: float(64) = 5.6;

var b64:complex(64) = 1.2 + 3.4i;
writeln( "b64 = ", b64);
a64 = 8.9i + f64;
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


writeln( "\ncomplex numbers");
var r64:complex;
writeln( "r64 = ", r64);
r64 = _max(r64.type);
writeln( "max(r64) = ", r64);
var t64: float = 5.6;

var s64:complex = 1.2 + 3.4i;
writeln( "s64 = ", s64);
r64 = 8.9i + t64;
writeln( "r64 = ", r64);

writeln( "r64 + s64 = ", r64 + s64);
writeln( "r64 + 2.0 = ", r64 + 2.0);
writeln( "r64 + 2.0i = ", r64 + 2.0i);
writeln( "r64 - s64 = ", r64 - s64);
writeln( "r64 - 2.0 = ", r64 - 2.0);
writeln( "r64 - 2.0i = ", r64 - 2.0i);
writeln( "r64 * s64 = ", r64 * s64);
writeln( "r64 / s64 = ", r64 / s64);
writeln( "r64 / 2.0 = ", r64 / 2.0);
writeln( "conjg(r64) = ", conjg(r64));


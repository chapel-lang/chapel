var x = -0.0;
var y = 0.0;

writeln(signbit(x:real(32)));
writeln(signbit(y:real(32)));

writeln(signbit(-NAN));
writeln(signbit(NAN));

writeln(signbit(x:real(64)));
writeln(signbit(y:real(64)));

writeln(signbit(-0.0));
writeln(signbit(+0.0));

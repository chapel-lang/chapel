use GMP;
var a = new BigInt(25);
var b: BigInt;
var c = new BigInt(5);
var ua : uint = 12;
var sa : int = -13;
b = a ** 3;
writeln(a, " ** 3 = ", b);

// TODO: Bitwise negation is not supported

b = a * ua;
writeln(a, " * ", ua, " = ", b);
b = a * sa;
writeln(a, " * ", sa, " = ", b);
b = a * c;
writeln(a, " * ", c, " = ", b);
b = a / ua;
writeln(a, " / ", ua, " = ", b);
b = a / sa;
writeln(a, " / ", sa, " = ", b);
b = a / c;
writeln(a, " / ", c, " = ", b);
b = a % ua;
writeln(a, " % ", ua, " = ", b);
b = a % sa;
writeln(a, " % ", sa, " = ", b);
b = a % c; 
writeln(a, " % ", c, " = ", b);

a = -7;
b = -a;
writeln("-(", a, ") = ", b);
b = +a;
writeln("+(", a, ") = ", b);
a = 25;
b = -a;
writeln("-(", a, ") = ", b);
b = +a;
writeln("+(", a, ") = ", b);

// TODO: BitShift operators << and >> not supported

b = a & c;
writeln(a, " & ", c, " = ", b);
b = a ^ c;
writeln(a, " ^ ", c, " = ", b);
b = a | c;
writeln(a, " | ", c, " = ", b);

b = a + ua;
writeln(a, " + ", ua, " = ", b);
b = a + sa;
writeln(a, " + ", sa, " = ", b);
b = a + c;
writeln(a, " + ", c, " = ", b);
b = a - ua;
writeln(a, " - ", ua, " = ", b);
b = a - sa;
writeln(a, " - ", sa, " = ", b);
b = a - c;
writeln(a, " - ", c, " = ", b);

a = 5;
if (a >= c) then writeln(a, " >= ", c);
if a <= c then writeln(a, " <= ", c);
if a > c then writeln(a, " > ", c);
if a < c then writeln(a, " < ", c);
a = -5;
if (a >= c) then writeln(a, " >= ", c);
if a <= c then writeln(a, " <= ", c);
if a > c then writeln(a, " > ", c);
if a < c then writeln(a, " < ", c);
a = 25;
if (a >= c) then writeln(a, " >= ", c);
if a <= c then writeln(a, " <= ", c);
if a > c then writeln(a, " > ", c);
if a < c then writeln(a, " < ", c);
if a >= 55 then writeln(a, " >= 55");
if a <= 55 then writeln(a, " <= 55");
if a > 55 then writeln(a, " > 55");
if a < 55 then writeln(a, " < 55");
if a >= -55 then writeln(a, " >= -55");
if a <= -55 then writeln(a, " <= -55");
if a > -55 then writeln(a, " > -55");
if a < -55 then writeln(a, " < -55");
c = 25;

if a == c then writeln(a, " == ", c);
if a != c then writeln(a, " != ", c);
a = 5;
if a == c then writeln(a, " == ", c);
if a != c then writeln(a, " != ", c);
if a == 5 then writeln(a, " == 5");
if a != 5 then writeln(a, " != 5");
a = 25;
if a == 5 then writeln(a, " == 5");
if a != 5 then writeln(a, " != 5");
a = -2;
if a == -2 then writeln(a, " == -2");
if a != -2 then writeln(a, " != -2");
a = 25;
if a == -2 then writeln(a, " == -2");
if a != -2 then writeln(a, " != -2");

// TODO: short-circuting logical AND && and OR || are not supported

writeln("DONE");

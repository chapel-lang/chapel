var integral1 = (1, 2, 3);
var integral2 = (4, 5, 6);
var general1 = (1, 3.14, 3.14+2.71i);
var general2 = (1, 2, 4i);
var generalNoCplx = (1, 2.7, 5);
var generalString = (1, "hello", 3.1+4i);

writeln("Plus");
writeln(integral1 + general1);
//writeln(general1 + generalString);

writeln();
writeln("Unary Plus");
writeln(+integral1);
writeln(+general1);

writeln();
writeln("Minus");
writeln(integral1 - general1);

writeln();
writeln("Unary Minus");
writeln(-integral1);
writeln(-general1);

writeln();
writeln("Times");
writeln(integral1 * general1);

writeln();
writeln("Divide");
writeln(integral1 / general1);

writeln();
writeln("Exponentiation");
writeln(integral1 ** generalNoCplx);

writeln();
writeln("Modulo");
writeln(integral1 % integral2);


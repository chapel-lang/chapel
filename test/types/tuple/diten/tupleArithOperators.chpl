var integral1 = (1, 2, 3);
var integral2 = (4, 5, 6);
var general1 = (1, 3.14, 3.14+2.71i);
var general2 = (1, 2, 4i);
var generalNoCplx = (1, 2.7, 5);
var generalString = (1, "hello", 3.1+4i);
var scalarInt = 2;
var scalarReal = 4.1;
var scalarCplx = 1 + 2i;

writeln("Plus");
writeln(integral1 + general1);
writeln(integral2 + scalarReal);
writeln(scalarInt + general2);
writeln(integral1 + scalarCplx);
writeln(general1 + generalString);

writeln();
writeln("Unary Plus");
writeln(+integral1);
writeln(+general1);

writeln();
writeln("Minus");
writeln(integral1 - general1);
writeln(integral2 - scalarReal);
writeln(scalarInt - general2);
writeln(integral1 - scalarCplx);

writeln();
writeln("Unary Minus");
writeln(-integral1);
writeln(-general1);

writeln();
writeln("Times");
writeln(integral1 * general1);
writeln(integral2 * scalarReal);
writeln(scalarInt * general2);
writeln(integral1 * scalarCplx);

writeln();
writeln("Divide");
writeln(integral1 / general1);
writeln(integral2 / scalarReal);
writeln(scalarInt / general2);
writeln(integral1 / scalarCplx);

writeln();
writeln("Exponentiation");
writeln(integral1 ** generalNoCplx);
writeln(generalNoCplx ** scalarReal);
writeln(scalarInt ** generalNoCplx);

writeln();
writeln("Modulo");
writeln(integral1 % integral2);
writeln(integral2 % scalarInt);
writeln(scalarInt % integral1);


var myuint1: uint(64) = 0x8000000000000000;
var myuint2: uint(64) = 0x8000000000000000u;

var mysint1: int(64) = 0x8000000000000000;
//var mysint2: int(64) = 0x8000000000000000u;

var myxint1 = 0x8000000000000000;
var myxint2 = 0x8000000000000000u;

writeln("myuint1 = ", myuint1);
writeln("myuint2 = ", myuint2);

writeln("mysint1 = ", mysint1);
//writeln("mysint2 = ", mysint2);

writeln("myxint1 = ", myxint1);
writeln("myxint2 = ", myxint2);
writeln();

writeln("myuint1 >> 62 = ", myuint1 >> 62);
writeln("myuint2 >> 62 = ", myuint2 >> 62);

writeln("mysint1 >> 62 = ", mysint1 >> 62);
//writeln("mysint2 >> 62 = ", mysint2 >> 62);

writeln("myxint1 >> 62 = ", myxint1 >> 62);
writeln("myxint2 >> 62 = ", myxint2 >> 62);

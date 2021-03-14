var myuint1: uint(64) = 0x8000000000000000;
var myuint2: uint(64) = 0x8000000000000000:uint(64);

//var mysint1: int(64) = 0x8000000000000000;
//var mysint2: int(64) = 0x8000000000000000:uint(64);
var mysint3: int(64) = 0x8000000000000000:int(64);
var mysint4: int(64) = 0x8000000000000000:uint(64):int(64);

var myxint1 = 0x8000000000000000;
var myxint2 = 0x8000000000000000:uint(64);
var myxint3 = 0x8000000000000000:uint(64):int(64);
var myxint4 = 0x8000000000000000:int(64);
var myxint5 = 0x8000000000000000:uint(64):uint(64);

writeln("myuint1 = ", myuint1);
writeln("myuint2 = ", myuint2);
writeln();

//writeln("mysint1 = ", mysint1);
//writeln("mysint2 = ", mysint2);
writeln("mysint3 = ", mysint3);
writeln("mysint4 = ", mysint4);
writeln();

writeln("myxint1 = ", myxint1);
writeln("myxint2 = ", myxint2);
writeln("myxint3 = ", myxint3);
writeln("myxint4 = ", myxint4);
writeln("myxint5 = ", myxint5);
writeln();

writeln("myuint1 >> 62 = ", myuint1 >> 62);
writeln("myuint2 >> 62 = ", myuint2 >> 62);
writeln();

//writeln("mysint1 >> 62 = ", mysint1 >> 62);
//writeln("mysint2 >> 62 = ", mysint2 >> 62);
writeln("mysint3 >> 62 = ", mysint3 >> 62);
writeln("mysint4 >> 62 = ", mysint4 >> 62);
writeln();

writeln("myxint1 >> 62 = ", myxint1 >> 62);
writeln("myxint2 >> 62 = ", myxint2 >> 62);
writeln("myxint3 >> 62 = ", myxint3 >> 62);
writeln("myxint4 >> 62 = ", myxint4 >> 62);
writeln("myxint5 >> 62 = ", myxint5 >> 62);
writeln();

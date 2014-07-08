var hom = (1, 2, 3);
var het = (1, 2.0, 3.1i);

var t1 = +hom;
writeln(t1);
writeln(typeToString(t1.type));

var t2 = +het;
writeln(t2);
writeln(typeToString(t2.type));

var t3 = -hom;
writeln(t3);
writeln(typeToString(t3.type));

var t4 = -het;
writeln(t4);
writeln(typeToString(t4.type));

var hetInt = (1: int(8), 2:int(16), 3:int(32));

var t5 = ~hom;
writeln(t5);
writeln(typeToString(t5.type));

var t6 = ~hetInt;
writeln(t6);
writeln(typeToString(t6.type));

var homBool = (true, false, true);
var hetBool = (true:bool(8), false:bool(16), true:bool(32));

var t7 = !homBool;
writeln(t7);
writeln(typeToString(t7.type));

var t8 = !hetBool;
writeln(t8);
writeln(typeToString(t8.type));

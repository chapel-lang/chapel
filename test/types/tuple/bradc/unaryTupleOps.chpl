var hom = (1, 2, 3);
var het = (1, 2.0, 3.1i);

var t1 = +hom;
writeln(t1);
writeln(t1.type:string);

var t2 = +het;
writeln(t2);
writeln(t2.type:string);

var t3 = -hom;
writeln(t3);
writeln(t3.type:string);

var t4 = -het;
writeln(t4);
writeln(t4.type:string);

var hetInt = (1: int(8), 2:int(16), 3:int(32));

var t5 = ~hom;
writeln(t5);
writeln(t5.type:string);

var t6 = ~hetInt;
writeln(t6);
writeln(t6.type:string);

var homBool = (true, false, true);
var hetBool = (true:bool(8), false:bool(16), true:bool(32));

var t7 = !homBool;
writeln(t7);
writeln(t7.type:string);

var t8 = !hetBool;
writeln(t8);
writeln(t8.type:string);

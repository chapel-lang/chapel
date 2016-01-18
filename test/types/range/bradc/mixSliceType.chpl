var r = 1:int(32)..10:int(32);
var r2 = 0:int(64)..9:int(64);

writeln("r is: ", r);
writeln("r2 is: ", r2);
writeln("r(r2) is: ", r(r2));
writeln("r2(r) is: ", r2(r));

writeln("r.type is: ", r.type:string);
writeln("r2.type is: ", r2.type:string);
writeln("r(r2).type is: ", r(r2).type:string);
writeln("r2(r).type is: ", r2(r).type:string);

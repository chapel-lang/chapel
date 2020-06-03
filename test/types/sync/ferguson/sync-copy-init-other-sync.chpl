var outersync: sync int = 1;
var foo1: sync int = outersync;
writeln(foo1.type:string);
writeln(foo1.isFull);
writeln(outersync.isFull);

var r : sync real = foo1;
writeln(r.type:string);
writeln(r.readXX());
writeln(r.isFull);
writeln(foo1.isFull);

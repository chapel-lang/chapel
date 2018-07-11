var outersync: sync int = 1;
var foo1: sync int = outersync;
writeln(foo1.type:string);
writeln(foo1.isFull);
writeln(outersync.isFull);

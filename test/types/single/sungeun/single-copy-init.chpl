var outersingle: single int = 1;
var foo1: single int = outersingle;
writeln(foo1.type:string);
writeln(foo1.isFull);
writeln(outersingle.isFull);

var r : single real = foo1;
writeln(r.type:string);
writeln(r.readXX());
writeln(r.isFull);
writeln(foo1.isFull);

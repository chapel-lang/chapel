var m = 10;
var n = 12;

writeln("m = ", m);
writeln("n = ", n);
writeln();

writeln("test: m &= n"); 
writeln("expected answer = 8");
m &= n;
writeln("actual answer = ", m);
writeln();

m = 10;
writeln("test: m ^= n");
writeln("expected answer = 6");
m ^= n;
writeln("actual answer = ", m);
writeln();

m = 10;
writeln("test: m |= n"); 
writeln("expected answer = 14");
m |= n;
writeln("actual answer = ", m);

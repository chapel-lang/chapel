var m = 10;
var n = 12;
var o =  0;
var p =  1;
var answer = 0;

write("m = ", m, "\n");
write("n = ", n, "\n");
write("o = ", o, "\n");
write("p = ", p, "\n\n");

writeln("test: m & n"); 
writeln("expected answer = 8");
answer = m & n;
writeln("actual answer = ", answer);
writeln();

writeln("test: m ^ n");
writeln("expected answer = 6");
answer = m ^ n;
writeln("actual answer = ", answer);
writeln();

writeln("test: m | n"); 
writeln("expected answer = 14");
answer = m | n;
writeln("actual answer = ", answer);
writeln();

writeln("test: o || p");
writeln("expected answer = 1");
answer = o != 0 || p != 0;
writeln("actual answer = ", answer);
writeln();

writeln("test: o || o");
writeln("expected answer = 0");
answer = o != 0 || o != 0;
writeln("actual answer = ", answer);
writeln();

writeln("test: p || p");
writeln("expected answer = 1");
answer = p != 0 || p != 0;
writeln("actual answer = ", answer);

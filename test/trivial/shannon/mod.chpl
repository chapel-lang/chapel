module test_mod {
var m = 5;
var n = 2;
var o = 10;
var answer = 0;

writeln("m = ", m);
writeln("n = ", n);
writeln("o = ", o);
writeln();

writeln("test: m % n");
writeln("expected answer = 1");
answer = m % n;
writeln("actual answer = ", answer);
writeln();

writeln("test: o % m");
writeln("expected answer = 0");
answer = o % m;
writeln("actual answer = ", answer);
}

config var n: int = 16;

var d1 : domain(1) = {1..n};
writeln("domain(1): \t\t", d1);
writeln("interior(-5): \t\t", d1.interior(-5));
writeln("interior(-4): \t\t", d1.interior(-4));
writeln("interior(-3): \t\t", d1.interior(-3));
writeln("interior(-2): \t\t", d1.interior(-2));
writeln("interior(-1): \t\t", d1.interior(-1));
writeln("interior(0): \t\t", d1.interior(0));
writeln("interior(1): \t\t", d1.interior(1));
writeln("interior(2): \t\t", d1.interior(2));
writeln("interior(3): \t\t", d1.interior(3));
writeln("interior(4): \t\t", d1.interior(4));
writeln("interior(5): \t\t", d1.interior(5));

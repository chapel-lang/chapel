config var n: int = 4;

var d1 : domain(1) = {1..n};
writeln("domain(1): \t\t", d1);
writeln("interior(0): \t\t", d1.interior(0));
writeln("interior(1): \t\t", d1.interior(1));
writeln("interior(2): \t\t", d1.interior(2));
writeln("interior(-3): \t\t", d1.interior(-3));
writeln("interior(-4): \t\t", d1.interior(-4));
writeln();

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t", d2);
writeln("interior(1, 3): \t", d2.interior(1, 3));
writeln("interior(-1, -3): \t", d2.interior(-1, -3));
writeln("interior(2, -1): \t", d2.interior(2, -1));
writeln("interior(-2, 1): \t", d2.interior(-2, 1));
writeln("interior(2, 0): \t", d2.interior(2, 0));
writeln("interior(0, 3): \t", d2.interior(0, 3));
writeln("interior(-3, 0): \t", d2.interior(-3, 0));
writeln("interior(0, -2): \t", d2.interior(0, -2));
writeln();

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t", d3);
writeln("interior(1, 2, 3): \t", d3.interior(1, 2, 3));
writeln("interior(-1, -2, -3): \t", d3.interior(-1, -2, -3));
writeln("interior(2, -1, 3): \t", d3.interior(2, -1, 3));
writeln("interior(-2, 1, -3): \t", d3.interior(-2, 1, -3));
writeln("interior(2, 0, -2): \t", d3.interior(2, 0, -2));
writeln("interior(0, 4, 0): \t", d3.interior(0, 4, 0));
writeln("interior(-3, 0, 3): \t", d3.interior(-3, 0, 3));
writeln("interior(0, -2, 0): \t", d3.interior(0, -2, 0));

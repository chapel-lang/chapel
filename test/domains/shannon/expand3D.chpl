config var n: int = 8;

var d1 : domain(1) = {1..n};
writeln("domain(1): \t\t", d1);
writeln("expand(1): \t\t", d1.expand(1));
writeln("expand(2): \t\t", d1.expand(2));
writeln("expand(101): \t\t", d1.expand(101));
writeln("expand(0): \t\t", d1.expand(0));
writeln("expand(-1): \t\t", d1.expand(-1));

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t", d2);
writeln("expand(1, 3): \t\t", d2.expand(1, 3));
writeln("expand(-1, -1): \t", d2.expand(-1, -1));
writeln("expand(2, -1): \t\t", d2.expand(2, -1));
writeln("expand(-1, 1): \t\t", d2.expand(-1, 1));
writeln("expand(2, 0): \t\t", d2.expand(2, 0));
writeln("expand(0, 3): \t\t", d2.expand(0, 3));
writeln("expand(-3, 0): \t\t", d2.expand(-3, 0));
writeln("expand(0, -1): \t\t", d2.expand(0, -1));

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t", d3);
writeln("expand(1, 3, 5): \t", d3.expand(1, 3, 5));
writeln("expand(-3, -1, 0): \t", d3.expand(-3, -1, 0));
writeln("expand(2, -1, 3): \t", d3.expand(2, -1, 3));
writeln("expand(-2, 1, -3): \t", d3.expand(-2, 1, -3));
writeln("expand(2, 0, -2): \t", d3.expand(2, 0, -2));
writeln("expand(0, 3, 0): \t", d3.expand(0, 3, 0));
writeln("expand(-1, 0, 3): \t", d3.expand(-1, 0, 3));
writeln("expand(0, -2, 0): \t", d3.expand(0, -2, 0));

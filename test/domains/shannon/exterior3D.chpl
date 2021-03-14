config var n: int = 4;

var d1 : domain(1) = {1..n};
writeln("domain(1): \t\t", d1);
writeln("exterior(1): \t\t", d1.exterior(1));
writeln("exterior(2): \t\t", d1.exterior(2));
writeln("exterior(0): \t\t", d1.exterior(0));
writeln("exterior(-1): \t\t", d1.exterior(-1));
writeln("exterior(-2): \t\t", d1.exterior(-2));

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t", d2);
writeln("exterior(1, 3): \t", d2.exterior(1, 3));
writeln("exterior(-1, -3): \t", d2.exterior(-1, -3));
writeln("exterior(2, -1): \t", d2.exterior(2, -1));
writeln("exterior(-2, 1): \t", d2.exterior(-2, 1));
writeln("exterior(2, 0): \t", d2.exterior(2, 0));
writeln("exterior(0, 3): \t", d2.exterior(0, 3));
writeln("exterior(-3, 0): \t", d2.exterior(-3, 0));
writeln("exterior(0, -2): \t", d2.exterior(0, -2));

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t", d3);
writeln("exterior(1, 3, 5): \t", d3.exterior(1, 3, 5));
writeln("exterior(-1, -3, -5): \t", d3.exterior(-1, -3, -5));
writeln("exterior(2, -1, 3): \t", d3.exterior(2, -1, 3));
writeln("exterior(-2, 1, -3): \t", d3.exterior(-2, 1, -3));
writeln("exterior(2, 0, -2): \t", d3.exterior(2, 0, -2));
writeln("exterior(0, 3, 0): \t", d3.exterior(0, 3, 0));
writeln("exterior(-3, 0, 3): \t", d3.exterior(-3, 0, 3));
writeln("exterior(0, -2, 0): \t", d3.exterior(0, -2, 0));

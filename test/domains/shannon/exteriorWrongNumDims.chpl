config var n : integer = 4;

var d3 : domain(3) = [1..n, 1..n, 1..n];
writeln("domain(3): \t\t", d3);
writeln("exterior(1, 2): ", d3.exterior(1, 2));

config var n: int = 4;

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t", d3);
writeln("expand(-1, -3, -5): \t", d3.expand(-1, -3, -5));

config var n : int = 4;

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t", d2);
writeln("expand(1): \t\t", d2.expand(1));
writeln();

var d3 : domain(3) = {0..2, 1..4, 2..8};
writeln("domain(3): \t\t", d3);
writeln("expand(2): \t\t", d3.expand(2));
writeln("expand(2, 2, 2): \t", d3.expand(2, 2, 2));


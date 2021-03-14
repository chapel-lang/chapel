config var n: int = 4;

var twice:  2*int = (-1, 3);
var thrice: 3*int = (-3, 0, 2);

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t", d2);
writeln("twice: \t\t\t", twice);
writeln("exterior(twice): \t", d2.exterior(twice));
writeln();

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t", d3);
writeln("thrice: \t\t", thrice);
writeln("exterior(thrice): \t", d3.exterior(thrice));

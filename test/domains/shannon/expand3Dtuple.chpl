config var n: int = 8;

var twice:  2*int = (-1, 1);
var thrice: 3*int = (-2, 0, 5);

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t", d2);
writeln("twice: \t\t\t", twice);
writeln("expand(twice): \t\t", d2.expand(twice));
writeln();

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t", d3);
writeln("thrice: \t\t", thrice);
writeln("expand(thrice): \t", d3.expand(thrice));

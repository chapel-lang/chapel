config var n : int = 4;

var twice:  2*int = (-1, 1);
var thrice: 3*int = (-1, 0, 1);

var d2 : domain(2) = {1..n, 1..n};
writeln("domain(2): \t\t\t", d2);
writeln("twice: \t\t\t\t", twice);
writeln("translate(twice): \t\t", d2.translate(twice));
writeln();

var d3 : domain(3) = {1..n, 1..n, 1..n};
writeln("domain(3): \t\t\t", d3);
writeln("thrice: \t\t\t", thrice);
writeln("translate(thrice): \t\t", d3.translate(thrice));

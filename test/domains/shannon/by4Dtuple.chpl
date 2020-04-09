config var n = 12;

var D4 : domain(4) = {1..n, 1..n, 1..n, 1..n};
var quad : 4*int = (1, 2, 3, 4);
var D4byquad = D4 by quad;
var D4by2 = D4 by 2;

writeln("quad: \t\t", quad);
writeln("D4: \t\t", D4);
writeln("D4byquad: \t", D4byquad);
writeln("D4by2: \t\t", D4by2);

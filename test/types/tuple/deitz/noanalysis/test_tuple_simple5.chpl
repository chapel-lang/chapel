var i : integer = 2;
var j : integer = 3;
var tuple : (integer, integer);

tuple = (4, 5);

writeln("i is ", i);
writeln("j is ", j);

writeln("tuple assignment");
(i, j) = tuple;

writeln("i is ", i);
writeln("j is ", j);

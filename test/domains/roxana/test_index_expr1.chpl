var i1 : index(2);
var i : integer = 0;
var j : integer = 0;

var tuple : (integer, integer);

i1 = (1,2);
tuple = i1;
(i, j) = tuple;

writeln("tuple.1 is ", tuple(1));
writeln("tuple.2 is ", tuple(2));
writeln("i is ", i);
writeln("j is ", j);
var i1 : index(2);
var i : int = 0;
var j : int = 0;

var tuple : (int, int);

i1 = (1,2);
tuple = i1;
(i, j) = tuple;

writeln("tuple.1 is ", tuple(0));
writeln("tuple.2 is ", tuple(1));
writeln("i is ", i);
writeln("j is ", j);

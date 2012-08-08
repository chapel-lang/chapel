var d = {1..5, 1..5, 1..5};
writeln(d.numIndices);
writeln({1..4, 1..3 by 2}.numIndices);
writeln({1..12 by 3}.numIndices);

var a: [1..10] int;
writeln(a.numElements);

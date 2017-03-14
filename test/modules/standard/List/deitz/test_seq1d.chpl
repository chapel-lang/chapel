use List;

var i : int = 2;
var j : int = 3;
var k : int = 4;

var s : list(int) = makeList( i, j, k );

writeln(s);

s.destroy();


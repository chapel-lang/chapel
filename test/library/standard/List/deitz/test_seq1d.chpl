use LinkedList;

var i : int = 2;
var j : int = 3;
var k : int = 4;

var s : linkedList(int) = makeList( i, j, k );

writeln(s);

s.destroy();


use List;

var s1 : list of int = makeList(1, 2, 3, 4);
var s2 : list of int = makeList(5, 6, 7, 8);

writeln(s1);
writeln(s2);

var ss : list of list of int = makeList(s1, s2);

writeln(ss);

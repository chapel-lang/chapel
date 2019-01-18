use List;

var s : list(int)  = makeList(1, 2, 3);
var t : (int, int) = (1, 2);

writeln(s);
writeln(t);

s.destroy();

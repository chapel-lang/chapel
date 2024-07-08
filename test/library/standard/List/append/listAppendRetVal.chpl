use List;

var l = new list(int);

var a: [1..10] int;
var b: [1..10 by 3] int;
var r = 3..8 by 4;

var l2 = new list(int);
for aa in a do l2.pushBack(aa);

writeln(l.pushBack(a));
writeln(l.pushBack(b));
writeln(l.pushBack(r));
writeln(l.pushBack(l2));

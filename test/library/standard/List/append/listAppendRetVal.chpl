use List;

var l = new list(int);

var a: [1..10] int;
var b: [1..10 by 3] int;
var r = 3..8 by 4;

var l2 = new list(int);
for aa in a do l2.append(aa);

writeln(l.append(a));
writeln(l.append(b));
writeln(l.append(r));
writeln(l.append(l2));

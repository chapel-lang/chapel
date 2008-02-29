use List;

var s1 : list(int) = makeList(3, 1, 4, 1, 5, 9);

writeln(s1);
s1.concat(makeList(2, 6, 5));
writeln(s1);
s1 = makeList(2, 7, 1, 8, 2, 8);
writeln(s1);

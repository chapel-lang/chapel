use Random;

var rs = new randomStream(int, 314159);

var a = [10, 11, 12, 13, 14, 15, 16, 17, 18, 19];

rs.permutation(a);
writeln(a);

permutation(a, 12345);
writeln(a);

permutation(a);

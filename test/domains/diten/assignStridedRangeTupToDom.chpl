var D1: domain(2, strides=strideKind.any);
D1 = (1..6 by 2, 1..6 by 2);
writeln(D1);

var D2: domain(int, parSafe=true);
D2 = (1..6 by 2, 2..6 by 2);
writeln(D2);

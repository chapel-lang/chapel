var A: [1..10] real = [i in 1..10] i;

writeln(A.localSlice(1..10 by 2));
writeln(A.localSlice({1..10 by 2}));

var B: [1..10:int(64)] real = [i in 1..10] i;

writeln(B.localSlice(1..10 by 2));
writeln(B.localSlice({1..10 by 2}));

A.localSlice({1..10 by 2}) = -1;
A.localSlice({2..10 by 2}) = -2;

B.localSlice(1..10 by 2) = -1;
B.localSlice(2..10 by 2) = -2;

writeln("A is: ", A);
writeln("B is: ", B);


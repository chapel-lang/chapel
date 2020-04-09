var A: [1..10] real = [i in 1..10] i;

writeln(A.localSlice({1..12 by 2}));

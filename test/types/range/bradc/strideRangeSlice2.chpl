const rOdd = 1..10 by 2;

writeln("rOdd is: ", rOdd);

var rOddSlice = rOdd(3..8 by 2);

writeln("rOddSlice = ", rOddSlice);

rOddSlice = rOdd(2..8 by 2);

writeln("rOddSlice = ", rOddSlice);

rOddSlice = rOdd(3..8);

writeln("rOddSlice = ", rOddSlice);

rOddSlice = rOdd(2..8);

writeln("rOddSlice = ", rOddSlice);

rOddSlice = rOdd(0..11);

writeln("rOddSlice = ", rOddSlice);

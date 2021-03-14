config const n = 10;

const D = {1..n, 1..n};
const D2 = D by (2,2);

writeln("D is: ", D);
writeln("D2 is: ", D2);

var D2Slice = D2[1..n by 2, 2..n by 2];

writeln("D2Slice is: ", D2Slice);

D2Slice = D2[1..n, 2..n];

writeln("D2Slice is: ", D2Slice);

D2Slice = D2[0..n-1, 2..n+1];

writeln("D2Slice is: ", D2Slice);

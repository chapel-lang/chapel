const D = {1..10};
const D2 = {1..10, 1..10};

var A: [D] real = [i in 1..10] i;
var A2: [D2] real = [(i,j) in D2] i + j/10.0;

writeln(A#2);
writeln(A#-2);
writeln(A2#(2,3));
writeln(A2#(-2,-3));

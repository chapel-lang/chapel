config var flag = true;

var D = {1..10, 1..10};
var D2 = D#(flag, !flag);
var D3 = D#(false, true);

writeln(D2);
writeln(D3);

var A: [D] real;

writeln(A#(flag, !flag));
writeln(A#(false, true));

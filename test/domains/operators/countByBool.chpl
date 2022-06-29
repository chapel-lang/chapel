config var flag = true;

var D = {1..10, 1..10};
var D2 = D#(flag, !flag);
var D3 = D#(false, true);

writeln(D2);
writeln(D3);

var A: [D] real;

// These will print a single element due to true == 1 in each
// dimension:
writeln(A#(flag, flag));
writeln(A#(true, true));

// Because each of these counts includes a false (0), each of these
// array views will effectively be empty, so the writeln()s will
// generate a blank line rather than anything interesting.
writeln(A#(flag, !flag));
writeln(A#(false, true));

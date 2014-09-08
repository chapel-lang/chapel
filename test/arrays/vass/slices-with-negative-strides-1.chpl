var Dom = {1..4,1..4};
var A,B: [Dom] real;
B=2;
A=1;

var AS => A[1..4 by -1, 1..4 by -2];
// the output might change in the future: writeln(AS.domain);
writeln(AS);
writeln();

var BS => B[1..4 by -1 , 1..4 by 2];
// the output might change in the future: writeln(BS.domain);
writeln(BS);
writeln();

// testing lower-level code
const ths = 1..4, oth = 1..4 by -1;

writeln(ths.boundsCheck(ths));
writeln(ths.boundsCheck(oth));
writeln(oth.boundsCheck(ths));
writeln(oth.boundsCheck(oth));
writeln();

const bt = ths, bo = oth;
writeln(bo);
writeln(bt.this(bo));
writeln(bt.member(bo));
writeln();

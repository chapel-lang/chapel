
var A, B: [1..2,1..3] int;

proc main() {
  var C = [i in zip(A,B)] i:(2*int); // cast to (2*int) is due to #10282
  writeln(C);
}


var D1 = 1..0; // <-- empty
var D2 = 1..2;
proc main {
  var A =
   [idx1 in D1]
    [idx2 in D2]
      idx1*100 + idx2;
  writeln(A);
}


var D1 = 1..0; // <-- empty
var D2 = 1..2;
proc main {
  var A =
   for idx1 in D1 do
    [idx2 in D2]
      idx1*100 + idx2;
  writeln(A);
}

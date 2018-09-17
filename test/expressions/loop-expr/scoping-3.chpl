// This is a variant on arrays/shapes/skyline.chpl
// where the nested for-expressions are in a field's init.

var D1 = {1..3};
var D2 = {1..2};

var X: [D1] [D2] int;

record RRR {
  var AAA =
   for idx1 in D1 do
    for idx2 in D2 do
      idx1 * 100 + idx2;
}

var rrr: RRR;
writeln(rrr);

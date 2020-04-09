// This is a variant on arrays/shapes/skyline.chpl
// where the nested for-expressions are in a field's init.
// Eventually we want AAA to be skyline as in the above test.

var D1 = {1..3};
var D2 = {1..2};

// These are here to ensure no regresions. (The compiler used to crash.)
const idx1 = 5.0;
const idx2 = 6.0;

record RRR {
  var AAA =
   for idx1 in D1 do
    for idx2 in D2 do
      idx1 * 100 + idx2;
}

var rrr: RRR;
writeln(rrr);

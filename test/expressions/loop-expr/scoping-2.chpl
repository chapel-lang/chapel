// This is a variant on scoping-2.chpl
// with global variables named like for-exprs' indices.


var D1 = {1..3};
var D2 = {1..2};

const idx1 = 5.0;
const idx2 = 6.0;

record RRR {
  var AAA =
   for idx1 in D1 do
    for idx2 in D2 do
      1;
}

var rrr: RRR;
writeln(rrr);

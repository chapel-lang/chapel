enum color {red, green, blue};
use color;


// Cast a range such that the stride is not known at compile time
proc toAny(r) do return r: range(r.idxType, r.bounds, strideKind.any);


inline proc testRange(r){
  // Test first last of the range
  // Also test the same range when the stride is not known at compile time
  writeln(r.first);
  writeln(r.last);
  var rAny = toAny(r);
  writeln(rAny.first);
  writeln(rAny.last);
}

var rgb = red..blue;
var rdd = red..;
var ddb = ..blue;
var dd: range(color, boundKind.neither);
var negrgb = red..blue by -1;
var negrdd = red.. by -1;
var negddb = ..blue by -1;
var negdd = dd by -1;
var rdd2 = rdd by 2;
var negrdd2 = rdd by -2;
var ddb2 = ddb by 2;
var negddb2 = ddb by -2;

testRange(rgb);
testRange(rdd);
testRange(ddb);
testRange(dd);
testRange(negrgb);
testRange(negrdd);
testRange(negddb);
testRange(negdd);
testRange(rdd2);
testRange(negrdd2);
testRange(ddb2);
testRange(negddb2);

{
var ft = false..true;
var fdd = false..;
var ddt = ..true;
var dd: range(bool, boundKind.neither);
var negft = false..true by -1;
var negfdd = false.. by -1;
var negddt = ..true by -1;
var negdd = dd by -1;
var fdd2 = fdd by 2;
var negfdd2 = fdd by -2;
var ddt2 = ddt by 2;
var negddt2 = ddt by -2;


testRange(ft);
testRange(fdd);
testRange(ddt);
testRange(dd);
testRange(negft);
testRange(negfdd);
testRange(negddt);
testRange(negdd);
testRange(fdd2);
testRange(negfdd2);
testRange(ddt2);
testRange(negddt2);

}

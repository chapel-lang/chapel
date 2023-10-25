// Test code copied from the following files:
// test/unstable/rangeUnboundedEnumBoolFirstLast.chpl
// test/unstable/llvmMetadata.chpl
// test/unstable/blockDistArgs.chpl
// test/unstable/BigInteger/unstableDiv2Exp.chpl
// test/unstable/unstableScans.chpl
// test/unstable/Math/unstableConstants.chpl


// Dummy config consts used by prediff to differentiate .good
config const testType = "";

// test/unstable/rangeUnboundedEnumBoolFirstLast.chpl
enum color {red, green, blue};
use color;


// Cast a range such that the stride is not known at compile time
proc toAny(r) do return r: range(r.idxType, r.bounds, strideKind.any);


inline proc testRange(r){
  // Test first last of the range
  // Also test the same range when the stride is not known at compile time
  writeln(r);
  writeln("Has First ", r.hasFirst());
  writeln("Has Last ", r.hasLast());
  writeln("Is empty: ", r.isEmpty());
  if !r.isEmpty() {
    writeln("First ", r.first);
    writeln("Last ", r.last);
  }
  var rAny = toAny(r);
  writeln("Any Has First ", rAny.hasFirst());
  writeln("Any Has Last ", rAny.hasLast());
  writeln("Any Is empty: ", rAny.isEmpty());
  if !r.isEmpty() {
    writeln("Any First ", rAny.first);
    writeln("Any Last ", rAny.last);
  }
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
var bdd2ag = blue.. by 2 align green;
var ddr2ag = ..red by 2 align green;
var ddg2ab = ..green by 2 align blue;
var ddg3ab = ..green by 3 align blue;

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
testRange(bdd2ag);
testRange(ddr2ag);
testRange(ddg2ab);
testRange(ddg3ab);


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
var tdd2af = true .. by 2 align false;
var ddf2at = ..false by 2 align true;


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
testRange(tdd2af);
testRange(ddf2at);
}


// test/unstable/llvmMetadata.chpl
proc saxpy(a: ?elmType, X: [?d] a.type, Y: X.type) {
  var R: X.type = 0;

  // by setting "llvm.loop.isvectorized", the vectorizer will not run on this loop
  // it thinks the loop is already vectorized
  // this means assertVectorized will always assert true even if optimizations improve
  // this makes the test less fragile

  @llvm.assertVectorized()
  @llvm.metadata(("llvm.loop.isvectorized", true))
  foreach (r, x, y) in zip(R, X, Y) {
    r = a*x + y;
  }

  return R;
}


// test/unstable/blockDistArgs.chpl
var X, Y: [1..16] real = 1..16;
var R = saxpy(2.0, X, Y);
writeln(R);

use DSIUtil;
use BlockDist;

const Space = {1..100, 1..100};

const validDist = new blockDist(Space, Locales);

const badDist1 = new blockDist(Space, dataParTasksPerLocale=getDataParTasksPerLocale());
const badDist2 = new blockDist(Space, dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks());
const badDist3 = new blockDist(Space, dataParMinGranularity=getDataParMinGranularity());
const badDist4 = new blockDist(Space, rank=Space.rank);
const badDist5 = new blockDist(Space, idxType=Space.idxType);
const badDist6 = new blockDist(Space, sparseLayoutType=unmanaged DefaultDist);


// test/unstable/BigInteger/unstableDiv2Exp.chpl
use BigInteger;

var x: bigint = 6;
var res: bigint;
div2Exp(res, x, 2);
writeln(res);
rem2Exp(res, x, 2);
writeln(res);

// test/unstable/unstableScans.chpl
var A: [1..10] int = 1..10;

var B = + scan A;
var C = + scan [a in A] a;
var D = + scan A[1..10];
var E = maxloc scan zip(A, A.domain);

writeln(A);
writeln(B);
writeln(C);
writeln(D);
writeln(E);


// test/unstable/Math/unstableConstants.chpl
use Math;

writeln(isClose(halfPi, pi/2));     // Should trigger for halfPi
writeln(isClose(quarterPi, pi/4));  // Should trigger for quarterPi
writeln(isClose(reciprPi, 1/pi));   // Should trigger for reciprPi
// Should trigger for twiceReciprPi
writeln(isClose(twiceReciprPi, 2/pi));
// Should trigger for twiceReciprSqrtPi
writeln(isClose(twiceReciprSqrtPi, 2/sqrt(pi)));
writeln(isClose(sqrt2, sqrt(2)));   // Should trigger for sqrt2
writeln(isClose(reciprSqrt2, 1/sqrt(2))); // Should trigger for reciprSqrt2

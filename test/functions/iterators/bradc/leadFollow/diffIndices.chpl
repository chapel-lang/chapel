// This test checks the behavior of zippered iteration
// when the two arrays differ in idxType as well as in strideKind.

proc test(param s1, param s2) {

const r1 = 1:int(32)..(4*abs(s1)) by s1;
const r2 = 1:int(64)..(4*abs(s2)) by s2;

// ensure we are testing the int32/int64 case, as the range rules may change
compilerAssert(r1.idxType == int(32));

writeln("=== ", r1, " === ", r2, " ===");

var D1: domain(1, int(32), r1.strides) = r1;
var D2: domain(1, int(64), r2.strides) = r2;

var A: [D1] real;
var B: [D2] real = [i in D2] i;

writeln("B is: ", B);

forall (a,b) in zip(A,B) do
  a = b;

writeln("A is: ", A);

B = 0;

forall (b,a) in zip(B,A) do
  b = -a;

writeln("B is: ", B);

}

for param s1 in -2..2 do
  for param s2 in -2..2 do
    if s1!=0 && s2!=0 then
      test(s1, s2);

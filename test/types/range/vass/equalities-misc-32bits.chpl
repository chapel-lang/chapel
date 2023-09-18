// range comparisons should be oblivious of idxType

var r1 = ( 0..1 ): range(int(32));
var r2 = ( 0..1 ): range(uint(32));

writeln(r1==r2);
writeln(r1!=r2);
writeln( {r1} == {r2} );
writeln( {r1} != {r2} );

param si0 = 0: int(32);
param si1 = 1: int(32);
param uns0 = 0: uint(32);
param uns1 = 1: uint(32);

writeln( (si0..si1) == (uns0..uns1) );

writeln( {si0..si1} == {uns0..uns1} );
writeln( {si0..si1} != {uns0..uns1} );
writeln();

// range comparisons should discern the boundedness

const tests = (0..0, 0.., ..0, ..);
for param i1 in 0..3 do for param i2 in 0..3 {
  const cmp = (tests(i1) == tests(i2));
  writeln(cmp, if cmp == (i1 == i2) then " ok" else " error");
}

// This tests the utility functions in BlockCycDim against those in Math.

use BlockCycDim;

for a in -11 .. 11 {
  for b in 1:bcdPosInt .. 11:bcdPosInt {
    verify(a, b, mod(a,b), modP2(a,b), "mod");
    verify(a, b, divfloor(a,b), divfloorP2(a,b), "divfloor");
  }
}

proc verify(i, j, resMath, resBCD, name:string) {
  if resMath != resBCD then
    writeln(name, (i,j), "  expected ", resMath, "  got ", resBCD);
}

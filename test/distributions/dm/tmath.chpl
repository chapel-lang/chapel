// This tests the utility functions in BlockCycDim against those in Math.

use BlockCycDim;

testtype( int(64));
testtype(uint(64));
testtype( int(32));
testtype(uint(32));

proc testtype(type T) {
  writeln("testing ", typeToString(T));
  for x in (if isIntType(T) then -11 else 0) .. 11 {
    for y in 1 .. 11  {
      var a = x:T;
      var b = y:bcdPosInt;
      verify(a, b, mod(x,y), modP2(a,b), "mod");
      verify(a, b, divfloor(x,y), divfloorP2(a,b), "divfloor");
      verify(a, b, x * y, mulP2(a,b), "mul");
      verify(a, b, x / y, divP2(a,b), "div");
      verify(a, b, x % y, remP2(a,b), "rem");
    }
  }
}

proc verify(i, j, resMath, resBCD, name:string) {
  if resMath:int != resBCD:int then
    writeln(name, (i,j), "  expected ", resMath, "  got ", resBCD);
}

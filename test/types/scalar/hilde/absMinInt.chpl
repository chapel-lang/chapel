// absMinInt.chpl
//
// Demonstrates what abs(min(int)) returns.

param maxU2:uint(32) = 2**31;
param maxU1:uint(32) = maxU2 - 1 + maxU2;
param maxS1:int(32) = (maxU2-1):int(32);
param minS1:int(32) = -maxS1 - 1;

var aMinS1 = abs(minS1);
if aMinS1 < 0 then
  halt("abs should never return a negative number.");
writeln(aMinS1);

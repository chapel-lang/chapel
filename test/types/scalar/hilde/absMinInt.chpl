// absMinInt.chpl
//
// Demonstrates what abs(min(int)) returns.

param maxU2 = 2:uint(32)**31;
param maxU1 = maxU2 - 1 + maxU2;
param maxS1 = (maxU2-1):int(32);
param minS1 = -maxS1 - 1;

var aMinS1 = abs(minS1);
if aMinS1 < 0 then
  halt("abs should never return a negative number.");
writeln(aMinS1);

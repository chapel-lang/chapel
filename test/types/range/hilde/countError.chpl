// countError.chpl
//
// These should all generate errors.
//

param maxU2 = 2:uint**31;
param maxU1 = maxU2 - 1 + maxU2;
param maxS1 = (maxU2-1):int;
param minS1 = -maxS1 - 1;
param maxS2 = maxS1 / 2 + 2;
param minS2 = minS1 / 2;
param p64I2 = 2:uint(64)**63;
param p64I1 = (p64I2 - 1):int(64);
param n64I1 = -p64I1 - 1;
param u64I1 = p64I1:uint(64) + p64I2;

writeln(0..maxU2 by 2 # minS1);

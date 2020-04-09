// noCountBigUint.chpl
//
// Can't count off members of a big unsigned range.
// 
// This is probably related to needRangeCountType.chpl.
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

writeln(0..u64I1 # 0);
writeln(0..u64I1 # -2);
writeln(0..u64I1 # 2);
writeln(0..u64I1 # minS1);
writeln(0..u64I1 # maxS1);
writeln(0..u64I1 # n64I1);
writeln(0..u64I1 # p64I1);

writeln(0..u64I1 by 2 # 0);
writeln(0..u64I1 by 2 # minS2);
writeln(0..u64I1 by 2 # maxS2);
writeln(0..u64I1 by 2 # minS1);
writeln(0..u64I1 by 2 # maxS1);
writeln(0..u64I1 by 2 # maxU2);
writeln(0..u64I1 by 2 # maxU1);

writeln(0..u64I1 by minS2 # 0);
writeln(0..u64I1 by minS2 # minS2);
writeln(0..u64I1 by minS2 # maxS2);
writeln(0..u64I1 by minS2 # minS1);
writeln(0..u64I1 by minS2 # maxS1);
writeln(0..u64I1 by minS2 # maxU2);
writeln(0..u64I1 by minS2 # maxU1);

writeln(0..u64I1 by maxS2 # 0);
writeln(0..u64I1 by maxS2 # minS2);
writeln(0..u64I1 by maxS2 # maxS2);
writeln(0..u64I1 by maxS2 # minS1);
writeln(0..u64I1 by maxS2 # maxS1);
writeln(0..u64I1 by maxS2 # maxU2);
writeln(0..u64I1 by maxS2 # maxU1);

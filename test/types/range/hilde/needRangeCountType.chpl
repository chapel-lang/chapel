// needRangeCountType.chpl
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

// The length of this range comes out as zero.
// All of these counts ought to work.
writeln(minS1..maxS1 # minS2);
writeln(minS1..maxS1 # maxS2);
writeln(minS1..maxS1 # minS1);
writeln(minS1..maxS1 # maxS1);
writeln(minS1..maxS1 # maxU2);
writeln(minS1..maxS1 # maxU1);

writeln(minS1..maxS1 by 2 # 0);
writeln(minS1..maxS1 by 2 # minS2);
writeln(minS1..maxS1 by 2 # maxS2);
writeln(minS1..maxS1 by 2 # minS1);
writeln(minS1..maxS1 by 2 # maxS1);
writeln(minS1..maxS1 by 2 # maxU2);
// writeln(minS1..maxS1 by 2 # maxU1); // This shouldn't work.

writeln(minS1..maxS1 by minS2 # 0);
writeln(minS1..maxS1 by minS2 # -1);
writeln(minS1..maxS1 by minS2 # 1);
writeln(minS1..maxS1 by minS2 # -2);
writeln(minS1..maxS1 by minS2 # 2);
// writeln(minS1..maxS1 by minS2 # -3);
// writeln(minS1..maxS1 by minS2 # 3);

writeln(minS1..maxS1 by maxS2 # 0);
writeln(minS1..maxS1 by maxS2 # -1);
writeln(minS1..maxS1 by maxS2 # 1);
writeln(minS1..maxS1 by maxS2 # -2);
writeln(minS1..maxS1 by maxS2 # 2);
// writeln(minS1..maxS1 by maxS2 # -3);
// writeln(minS1..maxS1 by maxS2 # 3);

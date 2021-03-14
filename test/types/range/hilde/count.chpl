// count.chpl
//
// Tests the representation limits of the count operator.

param maxU2 = 2:uint(32)**31;
param maxU1 = maxU2 - 1 + maxU2;
param maxS1 = (maxU2-1):int(32);
param minS1 = -maxS1 - 1;
param maxS2 = maxS1 / 2 + 2;
param minS2 = minS1 / 2;
param p64I2 = 2:uint(64)**63;
param p64I1 = (p64I2 - 1):int(64);
param n64I1 = -p64I1 - 1;
param u64I1 = p64I1:uint(64) + p64I2;

writeln(1..10 # 0);
writeln(1..10 # 1);
writeln(1..10 # -1);
writeln(1..10 # 10);
writeln(1..10 # -10);

writeln(10..100 by 10 # 0);
writeln(10..100 by 10 # 1);
writeln(10..100 by 10 # -1);
writeln(10..100 by 10 # 10);
writeln(10..100 by 10 # -10);

writeln(-50..50 by 10 # 0);
writeln(-50..50 by 10 # 1);
writeln(-50..50 by 10 # -1);
writeln(-50..50 by 10 # 11);
writeln(-50..50 by 10 # -11);

writeln(0..maxU2 # 0);
writeln(0..maxU2 # minS2);
writeln(0..maxU2 # maxS2);
writeln(0..maxU2 # minS1);
writeln(0..maxU2 # maxS1);
writeln(0..maxU2 # maxU2);
// writeln(0..maxU2 # maxU1);  // Run-time error :-)

writeln(0..maxU2 by 2 # 0);
writeln(0..maxU2 by 2 # minS2);
writeln(0..maxU2 by 2 # maxS2);
// writeln(0..maxU2 by 2 # minS1);
// writeln(0..maxU2 by 2 # maxS1);
// writeln(0..maxU2 by 2 # maxU2);
// writeln(0..maxU2 by 2 # maxU1);

writeln(0..maxU2 by minS2 # 0);
writeln(0..maxU2 by minS2 # -1);
writeln(0..maxU2 by minS2 # 1);
writeln(0..maxU2 by minS2 # -2);
writeln(0..maxU2 by minS2 # 2);
writeln(0..maxU2 by minS2 # -3);
writeln(0..maxU2 by minS2 # 3);

writeln(0..maxU2 by maxS2 # 0);
writeln(0..maxU2 by maxS2 # -1);
writeln(0..maxU2 by maxS2 # 1);
writeln(0..maxU2 by maxS2 # -2);
writeln(0..maxU2 by maxS2 # 2);
// writeln(0..maxU2 by maxS2 # -3);
// writeln(0..maxU2 by maxS2 # 3);

// NOTE: The following may not be safe because they rely on signed integer
// wraparound which may not be dependable across compilers/platforms

// See needRangeCountType.chpl:
writeln(minS1..maxS1 # 0);
// NRCT:writeln(minS1..maxS1 # minS2);
// NRCT:writeln(minS1..maxS1 # maxS2);
// NRCT:writeln(minS1..maxS1 # minS1);
// NRCT:writeln(minS1..maxS1 # maxS1);
// NRCT:writeln(minS1..maxS1 # maxU2);
// NRCT:writeln(minS1..maxS1 # maxU1);

writeln(minS1..maxS1 by 2 # 0);
// NRCT:writeln(minS1..maxS1 by 2 # minS2);
// NRCT:writeln(minS1..maxS1 by 2 # maxS2);
// NRCT:writeln(minS1..maxS1 by 2 # minS1);
// NRCT:writeln(minS1..maxS1 by 2 # maxS1);
// NRCT:writeln(minS1..maxS1 by 2 # maxU2);
// NRCT:writeln(minS1..maxS1 by 2 # maxU1);

writeln(minS1..maxS1 by minS2 # 0);
// NRCT:writeln(minS1..maxS1 by minS2 # -1);
// NRCT:writeln(minS1..maxS1 by minS2 # 1);
// NRCT:writeln(minS1..maxS1 by minS2 # -2);
// NRCT:writeln(minS1..maxS1 by minS2 # 2);
// NRCT:writeln(minS1..maxS1 by minS2 # -3);
// NRCT:writeln(minS1..maxS1 by minS2 # 3);

writeln(minS1..maxS1 by maxS2 # 0);
// NRCT:writeln(minS1..maxS1 by maxS2 # -1);
// NRCT:writeln(minS1..maxS1 by maxS2 # 1);
// NRCT:writeln(minS1..maxS1 by maxS2 # -2);
// NRCT:writeln(minS1..maxS1 by maxS2 # 2);
// NRCT:writeln(minS1..maxS1 by maxS2 # -3);
// NRCT:writeln(minS1..maxS1 by maxS2 # 3);

// See noCountBigUint.chpl:
// NCBU:writeln(0..u64I1 # 0);
// NCBU:writeln(0..u64I1 # -2);
// NCBU:writeln(0..u64I1 # 2);
// NCBU:writeln(0..u64I1 # minS1);
// NCBU:writeln(0..u64I1 # maxS1);
// NCBU:writeln(0..u64I1 # n64I1);
// NCBU:writeln(0..u64I1 # p64I1);

// NCBU:writeln(0..u64I1 by 2 # 0);
// NCBU:writeln(0..u64I1 by 2 # minS2);
// NCBU:writeln(0..u64I1 by 2 # maxS2);
// NCBU:writeln(0..u64I1 by 2 # minS1);
// NCBU:writeln(0..u64I1 by 2 # maxS1);
// NCBU:writeln(0..u64I1 by 2 # maxU2);
// NCBU:writeln(0..u64I1 by 2 # maxU1);

// NCBU:writeln(0..u64I1 by minS2 # 0);
// NCBU:writeln(0..u64I1 by minS2 # minS2);
// NCBU:writeln(0..u64I1 by minS2 # maxS2);
// NCBU:writeln(0..u64I1 by minS2 # minS1);
// NCBU:writeln(0..u64I1 by minS2 # maxS1);
// NCBU:writeln(0..u64I1 by minS2 # maxU2);
// NCBU:writeln(0..u64I1 by minS2 # maxU1);

// NCBU:writeln(0..u64I1 by maxS2 # 0);
// NCBU:writeln(0..u64I1 by maxS2 # minS2);
// NCBU:writeln(0..u64I1 by maxS2 # maxS2);
// NCBU:writeln(0..u64I1 by maxS2 # minS1);
// NCBU:writeln(0..u64I1 by maxS2 # maxS1);
// NCBU:writeln(0..u64I1 by maxS2 # maxU2);
// NCBU:writeln(0..u64I1 by maxS2 # maxU1);

// See needRangeCountType.chpl:
writeln(n64I1..p64I1 # 0);
// NRCT:writeln(n64I1..p64I1 # -2);
// NRCT:writeln(n64I1..p64I1 # 2);
// NRCT:writeln(n64I1..p64I1 # minS1);
// NRCT:writeln(n64I1..p64I1 # maxS1);
// NRCT:writeln(n64I1..p64I1 # n64I1);
// NRCT:writeln(n64I1..p64I1 # p64I1);

writeln(n64I1..p64I1 by 2 # 0);
// NRCT:writeln(n64I1..p64I1 by 2 # minS2);
// NRCT:writeln(n64I1..p64I1 by 2 # maxS2);
// NRCT:writeln(n64I1..p64I1 by 2 # minS1);
// NRCT:writeln(n64I1..p64I1 by 2 # maxS1);
// NRCT:writeln(n64I1..p64I1 by 2 # maxU2);
// NRCT:writeln(n64I1..p64I1 by 2 # maxU1);

writeln(n64I1..p64I1 by minS2 # 0);
// NRCT:writeln(n64I1..p64I1 by minS2 # minS2);
// NRCT:writeln(n64I1..p64I1 by minS2 # maxS2);
// NRCT:writeln(n64I1..p64I1 by minS2 # minS1);
// NRCT:writeln(n64I1..p64I1 by minS2 # maxS1);
// NRCT:writeln(n64I1..p64I1 by minS2 # maxU2);
// NRCT:writeln(n64I1..p64I1 by minS2 # maxU1);

writeln(n64I1..p64I1 by maxS2 # 0);
// NRCT:writeln(n64I1..p64I1 by maxS2 # minS2);
// NRCT:writeln(n64I1..p64I1 by maxS2 # maxS2);
// NRCT:writeln(n64I1..p64I1 by maxS2 # minS1);
// NRCT:writeln(n64I1..p64I1 by maxS2 # maxS1);
// NRCT:writeln(n64I1..p64I1 by maxS2 # maxU2);
// NRCT:writeln(n64I1..p64I1 by maxS2 # maxU1);



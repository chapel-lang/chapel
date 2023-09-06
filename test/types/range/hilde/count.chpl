// count.chpl
//
// Tests the representation limits of the count operator.

param maxU2:uint(32) = 2**31;
param maxU1:uint(32) = maxU2 - 1 + maxU2;
param maxS1:int(32)  = (maxU2-1):int(32);
param minS1:int(32)  = -maxS1 - 1;
param maxS2:int(32)  = maxS1 / 2 + 2;
param minS2:int(32)  = minS1 / 2;
param p64I2:uint(64) = (2**63):uint(64);
param p64I1:int(64)  = (p64I2 - 1):int(64);
param n64I1:int(64)  = -p64I1 - 1;
param u64I1:uint(64) = p64I1:uint(64) + p64I2;

writeln("maxU2 = ", maxU2, " : ", maxU2.type:string);
writeln("maxU1 = ", maxU1, " : ", maxU1.type:string);
writeln("maxS1 = ", maxS1, " : ", maxS1.type:string);
writeln("maxS2 = ", maxS2, " : ", maxS2.type:string);
writeln("minS2 = ", minS2, " : ", minS2.type:string);
writeln("p64I2 = ", p64I2, " : ", p64I2.type:string);
writeln("p64I1 = ", p64I1, " : ", p64I1.type:string);
writeln("n64I1 = ", n64I1, " : ", n64I1.type:string);
writeln("u64I1 = ", u64I1, " : ", u64I1.type:string);

const zeroDDmaxU2 = 0:maxU2.type..maxU2;

writeln("A");
writeln(1..10 # 0);
writeln(1..10 # 1);
writeln(1..10 # -1);
writeln(1..10 # 10);
writeln(1..10 # -10);

writeln("B");
writeln(10..100 by 10 # 0);
writeln(10..100 by 10 # 1);
writeln(10..100 by 10 # -1);
writeln(10..100 by 10 # 10);
writeln(10..100 by 10 # -10);

writeln("C");
writeln(-50..50 by 10 # 0);
writeln(-50..50 by 10 # 1);
writeln(-50..50 by 10 # -1);
writeln(-50..50 by 10 # 11);
writeln(-50..50 by 10 # -11);

writeln("D");
writeln(zeroDDmaxU2 # 0);
writeln(zeroDDmaxU2 # minS2);
writeln(zeroDDmaxU2 # maxS2);
writeln(zeroDDmaxU2 # minS1);
writeln(zeroDDmaxU2 # maxS1);
writeln(zeroDDmaxU2 # maxU2);
// writeln(zeroDDmaxU2 # maxU1);  // Run-time error :-)

writeln("E");
writeln(zeroDDmaxU2 by 2 # 0);
writeln(zeroDDmaxU2 by 2 # minS2);
writeln(zeroDDmaxU2 by 2 # maxS2);
// writeln(zeroDDmaxU2 by 2 # minS1);
// writeln(zeroDDmaxU2 by 2 # maxS1);
// writeln(zeroDDmaxU2 by 2 # maxU2);
// writeln(zeroDDmaxU2 by 2 # maxU1);

writeln("F");
writeln(zeroDDmaxU2 by minS2 # 0);
writeln(zeroDDmaxU2 by minS2 # -1);
writeln(zeroDDmaxU2 by minS2 # 1);
writeln(zeroDDmaxU2 by minS2 # -2);
writeln(zeroDDmaxU2 by minS2 # 2);
writeln(zeroDDmaxU2 by minS2 # -3);
writeln(zeroDDmaxU2 by minS2 # 3);

writeln("G");
writeln(zeroDDmaxU2 by maxS2 # 0);
writeln(zeroDDmaxU2 by maxS2 # -1);
writeln(zeroDDmaxU2 by maxS2 # 1);
writeln(zeroDDmaxU2 by maxS2 # -2);
writeln(zeroDDmaxU2 by maxS2 # 2);
// writeln(zeroDDmaxU2 by maxS2 # -3);
// writeln(zeroDDmaxU2 by maxS2 # 3);

writeln("H");
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

writeln("I");
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



// by.chpl
//
// Tests some representational limits of the by operator.
//
// Commented-out cases generate compile-time errors.
//

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

writeln(0.. by -1);
writeln(0.. by minS1);
writeln(0.. by minS2);
writeln(0.. by maxS1);
writeln(0.. by maxS2);
// writeln(0.. by maxU2);
// writeln(0.. by maxU1);
// writeln(0.. by p64I2);
// writeln(0.. by p64I1);
// writeln(0.. by n64I1);
// writeln(0.. by u64I1);

writeln(-1.. by -1);
writeln(-1.. by minS1);
writeln(-1.. by minS2);
writeln(-1.. by maxS1);
writeln(-1.. by maxS2);
// writeln(-1.. by maxU2);
// writeln(-1.. by maxU1);
// writeln(-1.. by p64I2);
// writeln(-1.. by p64I1);
// writeln(-1.. by n64I1);
// writeln(-1.. by u64I1);

writeln(minS1.. by -1);
writeln(minS1.. by minS1);
writeln(minS1.. by minS2);
writeln(minS1.. by maxS1);
writeln(minS1.. by maxS2);
// writeln(minS1.. by maxU2);
// writeln(minS1.. by maxU1);
// writeln(minS1.. by p64I2);
// writeln(minS1.. by p64I1);
// writeln(minS1.. by n64I1);
// writeln(minS1.. by u64I1);

writeln(minS2.. by -1);
writeln(minS2.. by minS1);
writeln(minS2.. by minS2);
writeln(minS2.. by maxS1);
writeln(minS2.. by maxS2);
// writeln(minS2.. by maxU2);
// writeln(minS2.. by maxU1);
// writeln(minS2.. by p64I2);
// writeln(minS2.. by p64I1);
// writeln(minS2.. by n64I1);
// writeln(minS2.. by u64I1);

writeln(maxS1.. by -1);
writeln(maxS1.. by minS1);
writeln(maxS1.. by minS2);
writeln(maxS1.. by maxS1);
writeln(maxS1.. by maxS2);
// writeln(maxS1.. by maxU2);
// writeln(maxS1.. by maxU1);
// writeln(maxS1.. by p64I2);
// writeln(maxS1.. by p64I1);
// writeln(maxS1.. by n64I1);
// writeln(maxS1.. by u64I1);

writeln(maxS2.. by -1);
writeln(maxS2.. by minS1);
writeln(maxS2.. by minS2);
writeln(maxS2.. by maxS1);
writeln(maxS2.. by maxS2);
// writeln(maxS2.. by maxU2);
// writeln(maxS2.. by maxU1);
// writeln(maxS2.. by p64I2);
// writeln(maxS2.. by p64I1);
// writeln(maxS2.. by n64I1);
// writeln(maxS2.. by u64I1);

writeln(maxU2.. by -1);
writeln(maxU2.. by minS1);
writeln(maxU2.. by minS2);
writeln(maxU2.. by maxS1);
writeln(maxU2.. by maxS2);
// writeln(maxU2.. by maxU2); // value too large for int(32) stride, wraps
// writeln(maxU2.. by maxU1); // value too large for int(32) stride, wraps
// writeln(maxU2.. by p64I2);
// writeln(maxU2.. by p64I1);
// writeln(maxU2.. by n64I1);
// writeln(maxU2.. by u64I1);

writeln(maxU1.. by -1);
writeln(maxU1.. by minS1);
writeln(maxU1.. by minS2);
writeln(maxU1.. by maxS1);
writeln(maxU1.. by maxS2);
// writeln(maxU1.. by maxU2); // value too large for int(32) stride, wraps
// writeln(maxU1.. by maxU1); // value too large for int(32) stride, wraps
// writeln(maxU1.. by p64I2);
// writeln(maxU1.. by p64I1);
// writeln(maxU1.. by n64I1);
// writeln(maxU1.. by u64I1);

writeln(p64I2.. by -1);
writeln(p64I2.. by minS1);
writeln(p64I2.. by minS2);
writeln(p64I2.. by maxS1);
writeln(p64I2.. by maxS2);
writeln(p64I2.. by maxU2);
writeln(p64I2.. by maxU1);
// writeln(p64I2.. by p64I2); // value too large for int(64) stride, wraps
writeln(p64I2.. by p64I1);
writeln(p64I2.. by n64I1);
// writeln(p64I2.. by u64I1); // value too large for int(64) stride, wraps

writeln(p64I1.. by -1);
writeln(p64I1.. by minS1);
writeln(p64I1.. by minS2);
writeln(p64I1.. by maxS1);
writeln(p64I1.. by maxS2);
writeln(p64I1.. by maxU2);
writeln(p64I1.. by maxU1);
// writeln(p64I1.. by p64I2);
writeln(p64I1.. by p64I1);
writeln(p64I1.. by n64I1);
// writeln(p64I1.. by u64I1);

writeln(n64I1.. by -1);
writeln(n64I1.. by minS1);
writeln(n64I1.. by minS2);
writeln(n64I1.. by maxS1);
writeln(n64I1.. by maxS2);
writeln(n64I1.. by maxU2);
writeln(n64I1.. by maxU1);
// writeln(n64I1.. by p64I2);
writeln(n64I1.. by p64I1);
writeln(n64I1.. by n64I1);
// writeln(n64I1.. by u64I1);

writeln(u64I1.. by -1);
writeln(u64I1.. by minS1);
writeln(u64I1.. by minS2);
writeln(u64I1.. by maxS1);
writeln(u64I1.. by maxS2);
writeln(u64I1.. by maxU2);
writeln(u64I1.. by maxU1);
// writeln(u64I1.. by p64I2); // value too large for int(64) stride, wraps
writeln(u64I1.. by p64I1);
writeln(u64I1.. by n64I1);
// writeln(u64I1.. by u64I1); // value too large for int(64) stride, wraps


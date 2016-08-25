use GMPRandom;
config const printrandom = false;

// Tests/provides better code coverage of the GMP Random class

var r1 = new GMPRandom();
var r2 = new GMPRandom(true);
var a = new BigInt(10);
var r3 = new GMPRandom(a, 10, 10);
var r4 = new GMPRandom(8);
var r5 = new GMPRandom(r4);
var b = new BigInt(128);

r1.seed(a);
r2.seed(10);

var x = r1.urandomb(32);
if printrandom then writeln(x);
x = r2.urandomm(1024);
if printrandom then writeln(x);
r3.urandomb(a, 32);
if printrandom then writeln(a);
r4.urandomm(a, b);
if printrandom then writeln(a);
r5.rrandomb(a, 4);
if printrandom then writeln(a);

delete r1;
delete r2;
delete r3;
delete r4;
delete r5;

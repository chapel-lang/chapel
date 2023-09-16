use BigInteger;
use Math;

// This test is to check and compare the behavior of mod when dealing with
// negative numbers.
var a1 = 5;
var a2 = 7;
var a3: bigint = a1;
var a4: bigint = a2;

var b1 = -2;
var b2 = -3;
var b3 = -4;
var b4 = -5;
var b5 = -6;

// Check %
writeln("% cases");

var percentRes1 = a1 % b1; // 5 mod -2
writeln(percentRes1); // 1
var percentRes2 = a1 % b2; // 5 mod -3
writeln(percentRes2); // 2
var percentRes3 = a1 % b3; // 5 mod -4
writeln(percentRes3); // 1
var percentRes4 = a1 % b4; // 5 mod -5
writeln(percentRes4); // 0
var percentRes5 = a1 % b5; // 5 mod -6
writeln(percentRes5); // 5
var percentRes6 = a2 % b2; // 7 mod -3
writeln(percentRes6); // 1
var percentRes7 = a2 % b3; // 7 mod -4
writeln(percentRes7); // 3
var percentRes8 = a2 % b4; // 7 mod -5
writeln(percentRes8); // 2


// Check mod
writeln("mod() cases");

var modRes1 = mod(a1, b1); // 5 mod -2
writeln(modRes1); // -1
var modRes2 = mod(a1, b2); // 5 mod -3
writeln(modRes2); // -1
var modRes3 = mod(a1, b3); // 5 mod -4
writeln(modRes3); // -3
var modRes4 = mod(a1, b4); // 5 mod -5
writeln(modRes4); // 0
var modRes5 = mod(a1, b5); // 5 mod -6
writeln(modRes5); // -1
var modRes6 = mod(a2, b2); // 7 mod -3
writeln(modRes6); // -2
var modRes7 = mod(a2, b3); // 7 mod -4
writeln(modRes7); // -1
var modRes8 = mod(a2, b4); // 7 mod -5
writeln(modRes8); // -3


// Check bigint.%
writeln("bigint.% cases");

var bigPercentRes1 = a3 % b1; // 5 mod -2
writeln(bigPercentRes1); // 1
var bigPercentRes2 = a3 % b2; // 5 mod -3
writeln(bigPercentRes2); // 2
var bigPercentRes3 = a3 % b3; // 5 mod -4
writeln(bigPercentRes3); // 1
var bigPercentRes4 = a3 % b4; // 5 mod -5
writeln(bigPercentRes4); // 0
var bigPercentRes5 = a3 % b5; // 5 mod -6
writeln(bigPercentRes5); // 5
var bigPercentRes6 = a4 % b2; // 7 mod -3
writeln(bigPercentRes6); // 1
var bigPercentRes7 = a4 % b3; // 7 mod -4
writeln(bigPercentRes7); // 3
var bigPercentRes8 = a4 % b4; // 7 mod -5
writeln(bigPercentRes8); // 2


// Check bigint.mod
writeln("bigint.mod() cases");

var bigModRes: bigint;
mod(bigModRes, a3, b1: bigint); // 5 mod -2
writeln(bigModRes); // -1
mod(bigModRes, a3, b2: bigint); // 5 mod -3
writeln(bigModRes); // -1
mod(bigModRes, a3, b3: bigint); // 5 mod -4
writeln(bigModRes); // -3
mod(bigModRes, a3, b4: bigint); // 5 mod -5
writeln(bigModRes); // 0
mod(bigModRes, a3, b5: bigint); // 5 mod -6
writeln(bigModRes); // -1
mod(bigModRes, a4, b2: bigint); // 7 mod -3
writeln(bigModRes); // -2
mod(bigModRes, a4, b3: bigint); // 7 mod -4
writeln(bigModRes); // -1
mod(bigModRes, a4, b4: bigint); // 7 mod -5
writeln(bigModRes); // -3

// Check bigint.rem
writeln("bigint.rem() cases");
var bigRemRes: bigint;
rem(bigRemRes, a3, b1: bigint); // 5 rem -2
writeln(bigRemRes); // 1
rem(bigRemRes, a3, b2: bigint); // 5 rem -3
writeln(bigRemRes); // 2
rem(bigRemRes, a3, b3: bigint); // 5 rem -4
writeln(bigRemRes); // 1
rem(bigRemRes, a3, b4: bigint); // 5 rem -5
writeln(bigRemRes); // 0
rem(bigRemRes, a3, b5: bigint); // 5 rem -6
writeln(bigRemRes); // 5
rem(bigRemRes, a4, b2: bigint); // 7 rem -3
writeln(bigRemRes); // 1
rem(bigRemRes, a4, b3: bigint); // 7 rem -4
writeln(bigRemRes); // 3
rem(bigRemRes, a4, b4: bigint); // 7 rem -5
writeln(bigRemRes); // 2

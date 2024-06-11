// non-param numeric types
var i8: int(8) = 1;
var i16: int(16) = 1;
var i32: int(32) = 1;
var i64: int(64) = 1;
assert(sgn(i8) == 1);
assert(sgn(i16) == 1);
assert(sgn(i32) == 1);
assert(sgn(i64) == 1);

var u8: uint(8) = 1;
var u16: uint(16) = 1;
var u32: uint(32) = 1;
var u64: uint(64) = 1;
assert(sgn(u8) == 1);
assert(sgn(u16) == 1);
assert(sgn(u32) == 1);
assert(sgn(u64) == 1);

var r32: real(32) = 1;
var r64: real(64) = 1;
assert(sgn(r32) == 1);
assert(sgn(r64) == 1);


// param numeric types
param pi8: int(8) = 1;
param pi16: int(16) = 1;
param pi32: int(32) = 1;
param pi64: int(64) = 1;
assert(sgn(pi8) == 1);
assert(sgn(pi16) == 1);
assert(sgn(pi32) == 1);
assert(sgn(pi64) == 1);

param pu8: uint(8) = 1;
param pu16: uint(16) = 1;
param pu32: uint(32) = 1;
param pu64: uint(64) = 1;
assert(sgn(pu8) == 1);
assert(sgn(pu16) == 1);
assert(sgn(pu32) == 1);
assert(sgn(pu64) == 1);

param pr32: real(32) = 1;
param pr64: real(64) = 1;
assert(sgn(pr32) == 1);
assert(sgn(pr64) == 1);

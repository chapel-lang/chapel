// DO NOT EDIT - THIS FILE IS GENERATED AUTOMATICALLY.
// Verify that an error is reported when assigning to a would-be formal.
enum EnumType {
  eFirst,
  eSecond,
  eLast
}
record RecordSmall {
  var xxx: int;
}
const rSmall = new RecordSmall(xxx=600033);
record RecordLarge {
  var yy01, yy02, yy03, yy04, yy05, yy06, yy07, yy08, yy09, yy10,
      yy11, yy12, yy13, yy14, yy15, yy16, yy17, yy18, yy19, yy20: int;
}
const rLarge = new RecordLarge(yy01=30001, yy07=30007, yy20=30020);
class ClassType {
  var zzz: int;
}
const cInstance = new ClassType(44444444);
union UnionType {
  var ufield111, ufield222: int;
}
var uInstanceVar: UnionType;
uInstanceVar.ufield222 = 339933;
const uInstance = uInstanceVar;
type DomType1 = domain(1);
const init1dom = {7770..7771};
type DomType2 = domain(2);
const init2dom = {110001..110002, 330004..330005};
type ArrType1 = [init1dom] int;
const init1arr: ArrType1 = 11011;
type ArrType2 = [init2dom] int;
const init2arr: ArrType2 = 33033;
// declare all the variables
var b0: bool;
var b8: bool(8);
var b16: bool(16);
var b32: bool(32);
var b64: bool(64);
var u8: uint(8);
var u16: uint(16);
var u32: uint(32);
var u64: uint(64);
var i8: int(8);
var i16: int(16);
var i32: int(32);
var i64: int(64);
var r32: real(32);
var r64: real(64);
var m32: imag(32);
var m64: imag(64);
var z64: complex(64);
var z128: complex(128);
/* no strings for now */
var enm: EnumType;
/* no records for now */
/* no unions for now */
var cls: ClassType;
/* no tuples for now */
var dom1: DomType1;
var dom2: DomType2;
var arr1: ArrType1;
var arr2: ArrType2;
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funBlank(
 b0: bool,
 b8: bool(8),
 b16: bool(16),
 b32: bool(32),
 b64: bool(64),
 u8: uint(8),
 u16: uint(16),
 u32: uint(32),
 u64: uint(64),
 i8: int(8),
 i16: int(16),
 i32: int(32),
 i64: int(64),
 r32: real(32),
 r64: real(64),
 m32: imag(32),
 m64: imag(64),
 z64: complex(64),
 z128: complex(128),
/* no strings for now */
 enm: EnumType,
/* no records for now */
/* no unions for now */
 cls: ClassType,
/* no tuples for now */
 dom1: DomType1,
 dom2: DomType2,
 arr1: ArrType1,
 arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funBlank(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funConst(
const b0: bool,
const b8: bool(8),
const b16: bool(16),
const b32: bool(32),
const b64: bool(64),
const u8: uint(8),
const u16: uint(16),
const u32: uint(32),
const u64: uint(64),
const i8: int(8),
const i16: int(16),
const i32: int(32),
const i64: int(64),
const r32: real(32),
const r64: real(64),
const m32: imag(32),
const m64: imag(64),
const z64: complex(64),
const z128: complex(128),
/* no strings for now */
const enm: EnumType,
/* no records for now */
/* no unions for now */
const cls: ClassType,
/* no tuples for now */
const dom1: DomType1,
const dom2: DomType2,
const arr1: ArrType1,
const arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funConst(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funConstIn(
const in b0: bool,
const in b8: bool(8),
const in b16: bool(16),
const in b32: bool(32),
const in b64: bool(64),
const in u8: uint(8),
const in u16: uint(16),
const in u32: uint(32),
const in u64: uint(64),
const in i8: int(8),
const in i16: int(16),
const in i32: int(32),
const in i64: int(64),
const in r32: real(32),
const in r64: real(64),
const in m32: imag(32),
const in m64: imag(64),
const in z64: complex(64),
const in z128: complex(128),
/* no strings for now */
const in enm: EnumType,
/* no records for now */
/* no unions for now */
const in cls: ClassType,
/* no tuples for now */
const in dom1: DomType1,
const in dom2: DomType2,
const in arr1: ArrType1,
const in arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funConstIn(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funIn(
in b0: bool,
in b8: bool(8),
in b16: bool(16),
in b32: bool(32),
in b64: bool(64),
in u8: uint(8),
in u16: uint(16),
in u32: uint(32),
in u64: uint(64),
in i8: int(8),
in i16: int(16),
in i32: int(32),
in i64: int(64),
in r32: real(32),
in r64: real(64),
in m32: imag(32),
in m64: imag(64),
in z64: complex(64),
in z128: complex(128),
/* no strings for now */
in enm: EnumType,
/* no records for now */
/* no unions for now */
in cls: ClassType,
/* no tuples for now */
in dom1: DomType1,
in dom2: DomType2,
in arr1: ArrType1,
in arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funIn(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funInOut(
inout b0: bool,
inout b8: bool(8),
inout b16: bool(16),
inout b32: bool(32),
inout b64: bool(64),
inout u8: uint(8),
inout u16: uint(16),
inout u32: uint(32),
inout u64: uint(64),
inout i8: int(8),
inout i16: int(16),
inout i32: int(32),
inout i64: int(64),
inout r32: real(32),
inout r64: real(64),
inout m32: imag(32),
inout m64: imag(64),
inout z64: complex(64),
inout z128: complex(128),
/* no strings for now */
inout enm: EnumType,
/* no records for now */
/* no unions for now */
inout cls: ClassType,
/* no tuples for now */
inout dom1: DomType1,
inout dom2: DomType2,
inout arr1: ArrType1,
inout arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funInOut(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funOut(
out b0: bool,
out b8: bool(8),
out b16: bool(16),
out b32: bool(32),
out b64: bool(64),
out u8: uint(8),
out u16: uint(16),
out u32: uint(32),
out u64: uint(64),
out i8: int(8),
out i16: int(16),
out i32: int(32),
out i64: int(64),
out r32: real(32),
out r64: real(64),
out m32: imag(32),
out m64: imag(64),
out z64: complex(64),
out z128: complex(128),
/* no strings for now */
out enm: EnumType,
/* no records for now */
/* no unions for now */
out cls: ClassType,
/* no tuples for now */
out dom1: DomType1,
out dom2: DomType2,
out arr1: ArrType1,
out arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funOut(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funRef(
ref b0: bool,
ref b8: bool(8),
ref b16: bool(16),
ref b32: bool(32),
ref b64: bool(64),
ref u8: uint(8),
ref u16: uint(16),
ref u32: uint(32),
ref u64: uint(64),
ref i8: int(8),
ref i16: int(16),
ref i32: int(32),
ref i64: int(64),
ref r32: real(32),
ref r64: real(64),
ref m32: imag(32),
ref m64: imag(64),
ref z64: complex(64),
ref z128: complex(128),
/* no strings for now */
ref enm: EnumType,
/* no records for now */
/* no unions for now */
ref cls: ClassType,
/* no tuples for now */
ref dom1: DomType1,
ref dom2: DomType2,
ref arr1: ArrType1,
ref arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funRef(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done
// Create a function named FUNNAME with formals of INTENT
// that assigns to each formal, then call it.
// Requires #include "support-decls.cpp" and #include "var-decls.cpp"
// to set up the actuals.
// declare the function
proc funConstRef(
const ref b0: bool,
const ref b8: bool(8),
const ref b16: bool(16),
const ref b32: bool(32),
const ref b64: bool(64),
const ref u8: uint(8),
const ref u16: uint(16),
const ref u32: uint(32),
const ref u64: uint(64),
const ref i8: int(8),
const ref i16: int(16),
const ref i32: int(32),
const ref i64: int(64),
const ref r32: real(32),
const ref r64: real(64),
const ref m32: imag(32),
const ref m64: imag(64),
const ref z64: complex(64),
const ref z128: complex(128),
/* no strings for now */
const ref enm: EnumType,
/* no records for now */
/* no unions for now */
const ref cls: ClassType,
/* no tuples for now */
const ref dom1: DomType1,
const ref dom2: DomType2,
const ref arr1: ArrType1,
const ref arr2: ArrType2,
             lastarg: int)
{
// Verify that an error is reported when assigning to a would-be global.
// This needs #include "support-decls.cpp" and #include "var-decls.cpp".
// Can be placed in any scope.
begin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
cobegin {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
coforall jjjjj in 1..3 {
// assign to all the variables
b0 = true;
b8 = true;
b16 = true;
b32 = true;
b64 = true;
u8 = 78;
u16 = 716;
u32 = 70032;
u64 = 70064;
i8 = 88;
i16 = 816;
i32 = 80032;
i64 = 80064;
r32 = (5.032:real(32));
r64 = 5.064;
m32 = (6.032i:imag(32));
m64 = 6.064i;
z64 = ((-2+64i):complex(64));
z128 = (-2+128i);
/* no strings for now */
enm = EnumType.eLast;
/* no records for now */
/* no unions for now */
cls = cInstance;
/* no tuples for now */
dom1 = init1dom;
dom2 = init2dom;
arr1 = init1arr;
arr2 = init2arr;
}
}
// call the function
funConstRef(
b0,
b8,
b16,
b32,
b64,
u8,
u16,
u32,
u64,
i8,
i16,
i32,
i64,
r32,
r64,
m32,
m64,
z64,
z128,
/* no strings for now */
enm,
/* no records for now */
/* no unions for now */
cls,
/* no tuples for now */
dom1,
dom2,
arr1,
arr2,
        5);
// done

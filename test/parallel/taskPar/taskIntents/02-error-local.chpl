// DO NOT EDIT - THIS FILE IS GENERATED AUTOMATICALLY.
// Verify that an error is reported when assigning to a would-be local.
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
const cInstance = new unmanaged ClassType(44444444);
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
proc test() {
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
var cls: unmanaged ClassType?;
/* no tuples for now */
var dom1: DomType1;
var dom2: DomType2;
var arr1: ArrType1;
var arr2: ArrType2;
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
} //test()
test();

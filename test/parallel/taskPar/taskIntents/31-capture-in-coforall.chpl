// DO NOT EDIT - THIS FILE IS GENERATED AUTOMATICALLY.
// Test value capturing in a coforall.
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
/////////////////////////////////////////////////////////////////////////////
writeln("=== at the module level ===");
// Verify that values are captured upon a 'coforall'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.
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
var s$: sync int;
writeln("before coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
proc updateVars() {
  writeln("updateVars()");
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
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
}
coforall jjjjj in 1..2 {
  if jjjjj == 1
  {
    updateVars();
    writeln("coforall - after updateVars");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
    s$ = 1;
  }
  else
  {
    s$;
    writeln("coforall - jjjjj=2");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
  }
}
writeln("after coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
writeln("done");
/////////////////////////////////////////////////////////////////////////////
writeln("=== in a function ===");
proc test() {
// Verify that values are captured upon a 'coforall'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.
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
var s$: sync int;
writeln("before coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
proc updateVars() {
  writeln("updateVars()");
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
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
}
coforall jjjjj in 1..2 {
  if jjjjj == 1
  {
    updateVars();
    writeln("coforall - after updateVars");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
    s$ = 1;
  }
  else
  {
    s$;
    writeln("coforall - jjjjj=2");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
  }
}
writeln("after coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
writeln("done");
}
test();
/////////////////////////////////////////////////////////////////////////////
writeln("=== in a begin ===");
var sbegin$: sync int;
begin {
// Verify that values are captured upon a 'coforall'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.
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
var s$: sync int;
writeln("before coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
proc updateVars() {
  writeln("updateVars()");
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
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
}
coforall jjjjj in 1..2 {
  if jjjjj == 1
  {
    updateVars();
    writeln("coforall - after updateVars");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
    s$ = 1;
  }
  else
  {
    s$;
    writeln("coforall - jjjjj=2");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
  }
}
writeln("after coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
writeln("done");
  sbegin$ = 1;
}
sbegin$;
/////////////////////////////////////////////////////////////////////////////
writeln("=== in a cobegin ===");
cobegin {
  var iiiii: int;
  {
// Verify that values are captured upon a 'coforall'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.
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
var s$: sync int;
writeln("before coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
proc updateVars() {
  writeln("updateVars()");
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
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
}
coforall jjjjj in 1..2 {
  if jjjjj == 1
  {
    updateVars();
    writeln("coforall - after updateVars");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
    s$ = 1;
  }
  else
  {
    s$;
    writeln("coforall - jjjjj=2");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
  }
}
writeln("after coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
writeln("done");
  }
}
/////////////////////////////////////////////////////////////////////////////
writeln("=== in a coforall ===");
coforall iiiii in 1..3 {
  if iiiii == 2 {
// Verify that values are captured upon a 'coforall'.
// This needs #include "support-decls.cpp".
// Can be placed in any scope.
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
var s$: sync int;
writeln("before coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
proc updateVars() {
  writeln("updateVars()");
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
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
}
coforall jjjjj in 1..2 {
  if jjjjj == 1
  {
    updateVars();
    writeln("coforall - after updateVars");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
    s$ = 1;
  }
  else
  {
    s$;
    writeln("coforall - jjjjj=2");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
  }
}
writeln("after coforall");
// write out all the variables
writeln("b0", " ", b0);
writeln("b8", " ", b8);
writeln("b16", " ", b16);
writeln("b32", " ", b32);
writeln("b64", " ", b64);
writeln("u8", " ", u8);
writeln("u16", " ", u16);
writeln("u32", " ", u32);
writeln("u64", " ", u64);
writeln("i8", " ", i8);
writeln("i16", " ", i16);
writeln("i32", " ", i32);
writeln("i64", " ", i64);
writeln("r32", " ", r32);
writeln("r64", " ", r64);
writeln("m32", " ", m32);
writeln("m64", " ", m64);
writeln("z64", " ", z64);
writeln("z128", " ", z128);
/* no strings for now */
writeln("enm", " ", enm);
/* no records for now */
/* no unions for now */
writeln("cls", " ", cls);
/* no tuples for now */
writeln("dom1", " ", dom1);
writeln("dom2", " ", dom2);
writeln("arr1", " ", arr1);
writeln("arr2", " ", arr2);
writeln("done");
  }
}
/////////////////////////////////////////////////////////////////////////////
writeln("=== done ===");

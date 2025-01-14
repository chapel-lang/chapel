// This test covers the various param coercion/overload resolution
// cases that I ran into when extending param coercion to
// cover more cases.


// support methods, for --minimal-modules
extern type c_string = chpl_c_string;
inline operator :(x: int(?w), type t) do
    return __primitive("cast", t, x);
inline operator :(x: uint(?w), type t) do
    return __primitive("cast", t, x);

proc chpl__autoCopy(x:uint(?w)) do return x;
proc chpl__initCopy(x:uint(?w)) do return x;
proc chpl__initCopy(x:R(?)) { return x; }
proc chpl__autoDestroy(r:R(?)) { }

// minimal modules can't support Chapel strings, so define all string literals
// as CStrings.
extern {
    const char* cstr_int8 = "int(8)";
    const char* cstr_int16 = "int(16)";
    const char* cstr_int32 = "int(32)";
    const char* cstr_int = "int";
    const char* cstr_uint8 = "uint(8)";
    const char* cstr_uint16 = "uint(16)";
    const char* cstr_uint32 = "uint(32)";
    const char* cstr_uint = "uint";
    const char* cstr_expect = "%s expect %s, found %s\n";
    const char* cstr_p8 = "p8";
    const char* cstr_p16 = "p16";
    const char* cstr_p32 = "p32";
    const char* cstr_pu32 = "pu32";
    const char* cstr_pi = "pi";
    const char* cstr_pu = "pu";
    const char* cstr_v8 = "v8";
    const char* cstr_v16 = "v16";
    const char* cstr_vu = "vu";
    const char* cstr_foo1 = "foo1";
    const char* cstr_foo2 = "foo2";
    const char* cstr_foo3 = "foo3";
    const char* cstr_foo4 = "foo4";
    const char* cstr_foo5 = "foo5";
    const char* cstr_foo6 = "foo6";
    const char* cstr_foo7 = "foo7";
    const char* cstr_bar1 = "bar1";
    const char* cstr_bar2 = "bar2";
    const char* cstr_bar3 = "bar3";
    const char* cstr_bar4 = "bar4";
    const char* cstr_baz1 = "baz1";
    const char* cstr_boo1 = "boo1";
    const char* cstr_bok1 = "bok1";
    const char* cstr_bok2 = "bok2";
    const char* cstr_boz1 = "boz1";
}


// support methods that work with or without --minimal-modules

proc typeToCStringHelp(x:int(8)) {
  return cstr_int8;
}
proc typeToCStringHelp(x:int(16)) {
  return cstr_int16;
}
proc typeToCStringHelp(x:int(32)) {
  return cstr_int32;
}
proc typeToCStringHelp(x:int) {
  return cstr_int;
}
proc typeToCStringHelp(x:uint(8)) {
  return cstr_uint8;
}
proc typeToCStringHelp(x:uint(16)) {
  return cstr_uint16;
}
proc typeToCStringHelp(x:uint(32)) {
  return cstr_uint32;
}
proc typeToCStringHelp(x:uint) {
  return cstr_uint;
}

proc typeToCString(type t) {
  var x = 0:t;
  return typeToCStringHelp(x);
}

proc check(msg, type expect, type t) {
  extern proc printf(f, a, b, c);
  printf(cstr_expect, msg, typeToCString(expect), typeToCString(t));
}

proc foo(x:int(8), y:int(8)) {
  return 0:int(8);
}
proc foo(x:int(16), y:int(16)) {
  return 0:int(16);
}
proc foo(x:int(32), y:int(32)) {
  return 0:int(32);
}
proc foo(x:int(64), y:int(64)) {
  return 0:int(64);
}

proc foo(param x:int(8), param y:int(8)) param {
  return 0:int(8);
}
proc foo(param x:int(16), param y:int(16)) param {
  return 0:int(16);
}
proc foo(param x:int(32), param y:int(32)) param {
  return 0:int(32);
}
proc foo(param x:int(64), param y:int(64)) param {
  return 0:int(64);
}

proc bar(x:int(8), y:int(8)) {
  return 0:int(8);
}
proc bar(x:int(16), y:int(16)) {
  return 0:int(16);
}
proc bar(x:int(32), y:int(32)) {
  return 0:int(32);
}
proc bar(x:int(64), y:int(64)) {
  return 0:int(64);
}

proc baz(x:uint, y:int) {
  return 0:int(32);
}
proc baz(x:uint, param y:uint) {
  return 0:uint;
}

proc boo(x:int(8)) {
  return 0:int(8);
}

proc boo(x:int(32)) {
  return 0:int(32);
}

proc bok(x:int(8), y:int(8)) {
  return 0:int(8);
}
proc bok(x:int(16), y:int(16)) {
  return 0:int(16);
}
proc bok(x:int(32), y:int(32)) {
  return 0:int(32);
}
proc bok(x:int(64), y:int(64)) {
  return 0:int(64);
}

proc bok(x:uint(8), y:uint(8)) {
  return 0:uint(8);
}
proc bok(x:uint(16), y:uint(16)) {
  return 0:uint(16);
}
proc bok(x:uint(32), y:uint(32)) {
  return 0:uint(32);
}
proc bok(x:uint(64), y:uint(64)) {
  return 0:uint(64);
}

proc asSigned(type t) type where t == int(8) do return int(8);
proc asSigned(type t) type where t == int(16) do return int(16);
proc asSigned(type t) type where t == int(32) do return int(32);
proc asSigned(type t) type where t == int(64) do return int(64);
proc asSigned(type t) type where t == uint(8) do return int(8);
proc asSigned(type t) type where t == uint(16) do return int(16);
proc asSigned(type t) type where t == uint(32) do return int(32);
proc asSigned(type t) type where t == uint(64) do return int(64);
proc asUnsigned(type t) type where t == int(8) do return uint(8);
proc asUnsigned(type t) type where t == int(16) do return uint(16);
proc asUnsigned(type t) type where t == int(32) do return uint(32);
proc asUnsigned(type t) type where t == int(64) do return uint(64);
proc asUnsigned(type t) type where t == uint(8) do return uint(8);
proc asUnsigned(type t) type where t == uint(16) do return uint(16);
proc asUnsigned(type t) type where t == uint(32) do return uint(32);
proc asUnsigned(type t) type where t == uint(64) do return uint(64);

record R {
  var x;
}

proc boz(r:R(?), count:asSigned(r.x.type)) {
  return 1:r.x.type;
}

proc boz(r:R(?), count:asUnsigned(r.x.type)) {
  return 2:r.x.type;
}

proc boz(r:R(?), count) {
  return 0:uint(8);
}



param p8:int(8) = 8;
param p16:int(16) = 16;
param p32:int(32) = 32;
param pu32:uint(32) = 32;
param pi = 64;
param pu:uint = 64;
var v8 = p8;
var v16 = p16;
var vu32 = pu32;
var vu = pu;

check(cstr_p8, int(8), p8.type);
check(cstr_p16, int(16), p16.type);
check(cstr_p32, int(32), p32.type);
check(cstr_pu32, uint(32), pu32.type);
check(cstr_pi, int, pi.type);
check(cstr_pu, uint, pu.type);
check(cstr_v8, int(8), v8.type);
check(cstr_v16, int(16), v16.type);
check(cstr_vu, uint, vu.type);

var foo1 = foo(v8, pi);
check(cstr_foo1, int(8), foo1.type);

var foo2 = foo(p8, pi);
check(cstr_foo2, int(64), foo2.type);

var foo3 = foo(v8, p16);
check(cstr_foo3, int(8), foo3.type);

var foo4 = foo(p8, p16);
check(cstr_foo4, int(16), foo4.type);

var foo5 = foo(p32, pu32);
check(cstr_foo5, int(32), foo5.type);

var foo6 = foo(p32, pi);
check(cstr_foo6, int(64), foo6.type);

var foo7 = foo(p8, p32);
check(cstr_foo7, int(32), foo7.type);

var bar1 = bar(p8, p16);
check(cstr_bar1, int(16), bar1.type);

var bar2 = bar(p32, pu32);
check(cstr_bar2, int(32), bar2.type);

var bar3 = bar(p8, p16);
check(cstr_bar3, int(16), bar3.type);

var bar4 = bar(p8, p32);
check(cstr_bar4, int(32), bar4.type);

var baz1 = baz(vu, pi);
check(cstr_baz1, uint, baz1.type);

var boo1 = boo(pi);
check(cstr_boo1, int(8), boo1.type);

var bok1 = bok(pu, pi);
check(cstr_bok1, uint, bok1.type);

var bok2 = bok(p8, p32);
check(cstr_bok2, int(32), bok2.type);


var r = new R(vu32);
var boz1 = boz(r, pi);
check(cstr_boz1, uint(32), boz1.type);

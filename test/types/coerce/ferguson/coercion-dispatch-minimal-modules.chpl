// This test covers the various param coercion/overload resolution
// cases that I ran into when extending param coercion to
// cover more cases.


// support methods, for --minimal-modules

inline operator :(x: int(?w), type t)
    return __primitive(c"cast", t, x);
inline operator :(x: uint(?w), type t)
    return __primitive(c"cast", t, x);

proc chpl__autoCopy(x:uint(?w)) return x;
proc chpl__initCopy(x:uint(?w)) return x;
proc chpl__initCopy(x:R) { return x; }
proc chpl__autoDestroy(r:R) { }


// support methods that work with or without --minimal-modules

proc typeToCStringHelp(x:int(8)) {
  return c"int(8)";
}
proc typeToCStringHelp(x:int(16)) {
  return c"int(16)";
}
proc typeToCStringHelp(x:int(32)) {
  return c"int(32)";
}
proc typeToCStringHelp(x:int) {
  return c"int";
}
proc typeToCStringHelp(x:uint(8)) {
  return c"uint(8)";
}
proc typeToCStringHelp(x:uint(16)) {
  return c"uint(16)";
}
proc typeToCStringHelp(x:uint(32)) {
  return c"uint(32)";
}
proc typeToCStringHelp(x:uint) {
  return c"uint";
}

proc typeToCString(type t) {
  var x = 0:t;
  return typeToCStringHelp(x);
}

proc check(msg:c_string, type expect, type t) {
  extern proc printf(f:c_string, a:c_string, b:c_string, c:c_string);
  printf(c"%s expect %s, found %s\n", msg, typeToCString(expect), typeToCString(t));
}

proc foo(x:int(?w), y:int(w)) {
  return 0:int(w);
}
proc foo(param x:int(?w), param y:int(w)) param {
  return 0:int(w);
}

proc bar(x:int(?w), y:int(w)) {
  return 0:int(w);
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

proc bok(x:int(?w), y:int(w)) {
  return 0:int(w);
}
proc bok(x:uint(?w), y:uint(w)) {
  return 0:uint(w);
}


proc asSigned(type t) type where t == int(8) return int(8);
proc asSigned(type t) type where t == int(16) return int(16);
proc asSigned(type t) type where t == int(32) return int(32);
proc asSigned(type t) type where t == int(64) return int(64);
proc asSigned(type t) type where t == uint(8) return int(8);
proc asSigned(type t) type where t == uint(16) return int(16);
proc asSigned(type t) type where t == uint(32) return int(32);
proc asSigned(type t) type where t == uint(64) return int(64);
proc asUnsigned(type t) type where t == int(8) return uint(8);
proc asUnsigned(type t) type where t == int(16) return uint(16);
proc asUnsigned(type t) type where t == int(32) return uint(32);
proc asUnsigned(type t) type where t == int(64) return uint(64);
proc asUnsigned(type t) type where t == uint(8) return uint(8);
proc asUnsigned(type t) type where t == uint(16) return uint(16);
proc asUnsigned(type t) type where t == uint(32) return uint(32);
proc asUnsigned(type t) type where t == uint(64) return uint(64);

record R {
  var x;
}

proc boz(r:R, count:asSigned(r.x.type)) {
  return 1:r.x.type;
}

proc boz(r:R, count:asUnsigned(r.x.type)) {
  return 2:r.x.type; 
}

proc boz(r:R, count) {
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

check(c"p8", int(8), p8.type);
check(c"p16", int(16), p16.type);
check(c"p32", int(32), p32.type);
check(c"pu32", uint(32), pu32.type);
check(c"pi", int, pi.type);
check(c"pu", uint, pu.type);
check(c"v8", int(8), v8.type);
check(c"v16", int(16), v16.type);
check(c"vu", uint, vu.type);

var foo1 = foo(v8, pi);
check(c"foo1", int(8), foo1.type);

var foo2 = foo(p8, pi);
check(c"foo2", int(8), foo2.type);

var foo3 = foo(v8, p16);
check(c"foo3", int(8), foo3.type);

var foo4 = foo(p8, p16);
check(c"foo4", int(16), foo4.type);

var foo5 = foo(p32, pu32);
check(c"foo5", int(64), foo5.type);

var foo6 = foo(p32, pi);
check(c"foo6", int(32), foo6.type);

var foo7 = foo(p8, p32);
check(c"foo7", int(32), foo7.type);

var bar1 = bar(p8, p16);
check(c"bar1", int(16), bar1.type);

var bar2 = bar(p32, pu32);
check(c"bar2", int(64), bar2.type);

var bar3 = bar(p8, p16);
check(c"bar3", int(16), bar3.type);

var bar4 = bar(p8, p32);
check(c"bar4", int(32), bar4.type);

var baz1 = baz(vu, pi);
check(c"baz1", uint, baz1.type);

var boo1 = boo(pi);
check(c"boo1", int(8), boo1.type);

var bok1 = bok(pu, pi);
check(c"bok1", uint, bok1.type);

var bok2 = bok(p8, p32);
check(c"bok2", int(32), bok2.type);


var r = new R(vu32);
var boz1 = boz(r, pi);
check(c"boz1", uint(32), boz1.type);

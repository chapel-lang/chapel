class A {
  var x: int;
}

class B {
  var x: int;
}

class C {
  var x: int;
}

class D {
  var x: int;
}

class E {
  var x: int;
}

class F {
  var x: int;
}

class G {
  var x: int;
}

class H {
  var x: int;
}

class I {
  var x: int;
}

class J {
  var x: int;
}

class K {
  var x: int;
}

proc =(lhs: A, rhs: A) {
  lhs.x = rhs.x;
}

proc =(lhs: unmanaged B, rhs: B) {
  lhs.x = rhs.x;
}

proc =(lhs: borrowed C, rhs: C) {
  lhs.x = rhs.x;
}

proc =(lhs: shared D, rhs: D) {
  lhs.x = rhs.x;
}

proc =(lhs: owned E, rhs: E) {
  lhs.x = rhs.x;
}

proc =(ref lhs: F, rhs: F) {
  lhs.x = rhs.x;
}

proc =(ref lhs: owned G, rhs: G) {
  lhs.x = rhs.x;
}

proc =(ref lhs: borrowed H, rhs: H) {
  lhs.x = rhs.x;
}

proc =(ref lhs: I?, rhs: I) {
  lhs!.x = rhs.x;
}

proc =(ref lhs: owned J?, rhs: J) {
  lhs!.x = rhs.x;
}

proc =(ref lhs: borrowed K?, rhs: K) {
  lhs!.x = rhs.x;
}

proc =(ref lhs: A, rhs: int) {
  lhs.x = rhs;
}

proc =(ref lhs: A, rhs: R) {
  lhs.x = rhs.x;
}

record R {
  var x: int;
}
         
var myA = new A(42), myA2 = new A(33);
myA = myA2;
writeln(myA, myA2);

var myB = new unmanaged B(42), myB2 = new B(33);
myB = myB2;
writeln(myB, myB2);

var myC = new borrowed C(42), myC2 = new C(33);
myC = myC2;
writeln(myC, myC2);

var myD = new shared D(42), myD2 = new D(33);
myD = myD2;
writeln(myD, myD2);

var myE = new E(42), myE2 = new E(33);
myE = myE2;
writeln(myE, myE2);

var myF = new F(42), myF2 = new F(33);
myF = myF2;
writeln(myF, myF2);

var myG = new G(42), myG2 = new G(33);
myG = myG2;
writeln(myG, myG2);

var myH = new borrowed H(42), myH2 = new H(33);
myH = myH2;
writeln(myH, myH2);

var myI = new I(42), myI2 = new I(33);
myI = myI2;
writeln(myI, myI2);

var myJ: J? = new owned J(42), myJ2 = new J(33);
myJ = myJ2;
writeln(myJ, myJ2);

var myK = new K(42), myK2 = new K(33);
myK = myK2;
writeln(myK, myK2);

myA = 42;
writeln(myA);

myA = new R(33);
writeln(myA);

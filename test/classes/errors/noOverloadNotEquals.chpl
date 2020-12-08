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

proc !=(lhs: A, rhs: A) {
  return lhs.x != rhs.x;
}

proc !=(lhs: unmanaged B, rhs: B) {
  return lhs.x != rhs.x;
}

proc !=(lhs: borrowed C, rhs: C) {
  return lhs.x != rhs.x;
}

proc !=(lhs: shared D, rhs: D) {
  return lhs.x != rhs.x;
}

proc !=(lhs: owned E, rhs: E) {
  return lhs.x != rhs.x;
}

proc !=(ref lhs: F, rhs: F) {
  return lhs.x != rhs.x;
}

proc !=(ref lhs: owned G, rhs: G) {
  return lhs.x != rhs.x;
}

proc !=(ref lhs: borrowed H, rhs: H) {
  return lhs.x != rhs.x;
}

proc !=(ref lhs: I?, rhs: I) {
  return lhs!.x != rhs.x;
}

proc !=(ref lhs: owned J?, rhs: J) {
  return lhs!.x != rhs!.x;
}

proc !=(ref lhs: borrowed K?, rhs: K) {
  return lhs!.x != rhs!.x;
}

proc !=(ref lhs: A, rhs: int) {
  return lhs.x != rhs;
}

proc !=(ref lhs: A, rhs: R) {
  return lhs.x != rhs.x;
}

record R {
  var x: int;
}
         
var myA = new A(42), myA2 = new A(33);
writeln(myA != myA2);

var myB = new unmanaged B(42), myB2 = new B(33);
writeln(myB != myB2);

var myC = new borrowed C(42), myC2 = new C(33);
writeln(myC != myC2);

var myD = new shared D(42), myD2 = new D(33);
writeln(myD != myD2);

var myE = new E(42), myE2 = new E(33);
writeln(myE != myE2);

var myF = new F(42), myF2 = new F(33);
writeln(myF != myF2);

var myG = new G(42), myG2 = new G(33);
writeln(myG != myG2);

var myH = new borrowed H(42), myH2 = new H(33);
writeln(myH != myH2);

var myI: I? = new I(42), myI2 = new I(33);
writeln(myI != myI2);

var myJ: J? = new owned J(42), myJ2 = new J(33);
writeln(myJ != myJ2);

var myKtmp: K? = new K(42), myK: borrowed K? = myKtmp.borrow(), myK2 = new K(33);
writeln(myK != myK2);

writeln(myA != 42);

writeln(myA != new R(33));
  

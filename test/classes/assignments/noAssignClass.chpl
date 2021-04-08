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

operator =(lhs: A, rhs: A) {
  lhs.x = rhs.x;
}

operator =(lhs: unmanaged B, rhs: B) {
  lhs.x = rhs.x;
}

operator =(lhs: borrowed C, rhs: C) {
  lhs.x = rhs.x;
}

operator =(lhs: shared D, rhs: D) {
  lhs.x = rhs.x;
}

operator =(lhs: owned E, rhs: E) {
  lhs.x = rhs.x;
}

operator =(ref lhs: F, rhs: F) {
  lhs.x = rhs.x;
}

operator =(ref lhs: owned G, rhs: G) {
  lhs.x = rhs.x;
}

operator =(ref lhs: borrowed H, rhs: H) {
  lhs.x = rhs.x;
}

operator =(ref lhs: I?, rhs: I) {
  lhs!.x = rhs.x;
}

operator =(ref lhs: owned J?, rhs: J) {
  lhs!.x = rhs.x;
}

operator =(ref lhs: borrowed K?, rhs: K) {
  lhs!.x = rhs.x;
}

operator =(ref lhs: A, rhs: int) {
  lhs.x = rhs;
}

operator =(ref lhs: A, rhs: R) {
  lhs.x = rhs.x;
}

class L {
  var x: int;
}

class M {
  var x: int;
}

class N {
  var x: int;
}

class O {
  var x: int;
}

class P {
  var x: int;
}

class Q {
  var x: int;
}

class S {
  var x: int;
}

class T {
  var x: int;
}

class U {
  var x: int;
}

class V {
  var x: int;
}

class W {
  var x: int;
}

operator L.=(lhs: L, rhs: L) {
  lhs.x = rhs.x;
}

operator M.=(lhs: unmanaged M, rhs: M) {
  lhs.x = rhs.x;
}

operator N.=(lhs: borrowed N, rhs: N) {
  lhs.x = rhs.x;
}

operator O.=(lhs: shared O, rhs: O) {
  lhs.x = rhs.x;
}

operator P.=(lhs: owned P, rhs: P) {
  lhs.x = rhs.x;
}

operator Q.=(ref lhs: Q, rhs: Q) {
  lhs.x = rhs.x;
}

operator S.=(ref lhs: owned S, rhs: S) {
  lhs.x = rhs.x;
}

operator T.=(ref lhs: borrowed T, rhs: T) {
  lhs.x = rhs.x;
}

operator U.=(ref lhs: U?, rhs: U) {
  lhs!.x = rhs.x;
}

operator V.=(ref lhs: owned V?, rhs: V) {
  lhs!.x = rhs.x;
}

operator W.=(ref lhs: borrowed W?, rhs: W) {
  lhs!.x = rhs.x;
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

var myL = new L(42), myL2 = new L(33);
myL = myL2;
writeln(myL, myL2);

var myM = new unmanaged M(42), myM2 = new M(33);
myM = myM2;
writeln(myM, myM2);

var myN = new borrowed N(42), myN2 = new N(33);
myN = myN2;
writeln(myN, myN2);

var myO = new shared O(42), myO2 = new O(33);
myO = myO2;
writeln(myO, myO2);

var myP = new P(42), myP2 = new P(33);
myP = myP2;
writeln(myP, myP2);

var myQ = new Q(42), myQ2 = new Q(33);
myQ = myQ2;
writeln(myQ, myQ2);

var myS = new S(42), myS2 = new S(33);
myS = myS2;
writeln(myS, myS2);

var myT = new borrowed T(42), myT2 = new T(33);
myT = myT2;
writeln(myT, myT2);

var myU = new U(42), myU2 = new U(33);
myU = myU2;
writeln(myU, myU2);

var myV: V? = new owned V(42), myV2 = new V(33);
myV = myV2;
writeln(myV, myV2);

var myW = new W(42), myW2 = new W(33);
myW = myW2;
writeln(myW, myW2);

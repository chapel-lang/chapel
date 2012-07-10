class C {
  type t;

  var x: t;

  proc foo() {
    writeln("x.domain is: ", x.domain);
    writeln("x is: ", x);
    writeln();
  }
}

type arithArr = [1..3] real;
var arithC = new C(arithArr);
[i in 1..3] arithC.x(i) = i + i/10.0;
arithC.foo();
delete arithC;

var assocDom: domain(string);
type assocArr = [assocDom] real;
var assocC = new C(assocArr);
assocDom += "one";
assocC.x("one") = 1.1;
assocC.foo();
delete assocC;

var opaqueDom: domain(opaque);
type opaqueArr = [opaqueDom] real;
var opaqueC = new C(opaqueArr);
const newInd = opaqueDom.create();
opaqueC.x(newInd) = 1.1;
opaqueC.foo();
delete opaqueC;

var sparseDom: sparse subdomain({1..3});
type sparseArr = [sparseDom] real;
var sparseC = new C(sparseArr);
sparseDom += 2;
sparseC.x(2) = 2.2;
sparseC.foo();
delete sparseC;

enum probClass {S, W, A, B, C};
const enumDom: domain(probClass);
type enumArr = [enumDom] real;
var enumC = new C(enumArr);
[i in enumDom] enumC.x(i) = i:int + i:real/10.0;
enumC.foo();
delete enumC;

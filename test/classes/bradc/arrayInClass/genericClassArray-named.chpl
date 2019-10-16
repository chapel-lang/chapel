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
var arithC = new unmanaged C(arithArr);
[i in 1..3] arithC.x(i) = i + i/10.0;
arithC.foo();
delete arithC;

var assocDom: domain(string);
type assocArr = [assocDom] real;
var assocC = new unmanaged C(assocArr);
assocDom += "one";
assocC.x("one") = 1.1;
assocC.foo();
delete assocC;

var sparseDom: sparse subdomain({1..3});
type sparseArr = [sparseDom] real;
var sparseC = new unmanaged C(sparseArr);
sparseDom += 2;
sparseC.x(2) = 2.2;
sparseC.foo();
delete sparseC;

enum probClass {S=1, W, A, B, C};
const enumDom: domain(probClass) = probClass.S..probClass.C;
type enumArr = [enumDom] real;
var enumC = new unmanaged C(enumArr);
[i in enumDom] enumC.x(i) = i:int + i:real/10.0;
enumC.foo();
delete enumC;

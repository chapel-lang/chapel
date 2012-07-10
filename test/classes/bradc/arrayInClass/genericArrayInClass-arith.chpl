// declare class types

class ArithC {
  type t;

  var x: [1..3] t;
}

class AssocC {
  type t;

  var assocDom: domain(string);
  var x: [assocDom] t;
}

class OpaqueC {
  type t;

  var opaqueDom: domain(opaque);
  var x: [opaqueDom] t;
}

class SparseC {
  type t;
  
  var sparseDom: sparse subdomain({1..3});
  var x: [sparseDom] t;
}

enum probClass {S, W, A, B, C};

class EnumC {
  type t;
  
  var enumDom: domain(probClass);
  var x: [enumDom] t;
}

// generic print routine

proc foo(C) {
  writeln("C.x.domain is: ", C.x.domain);
  writeln("x is: ", C.x);
  writeln();
}

// declare class instances

const vecDom = {1..3};
type vec = [vecDom] real;

var myArithC = new ArithC(vec);
var myAssocC = new AssocC(vec);
var myOpaqueC = new OpaqueC(vec);
var mySparseC = new SparseC(vec);
var myEnumC = new EnumC(vec);

// initialize class instances

[i in myArithC.x.domain] [j in vecDom] myArithC.x(i)(j) = i + j/10.0;

myAssocC.assocDom += "two";
[j in vecDom] myAssocC.x("two")(j) = 2 + j/10.0;

const newInd = myOpaqueC.opaqueDom.create();
[j in vecDom] myOpaqueC.x(newInd)(j) = 2 + j/10.0;

mySparseC.sparseDom += 2;
[j in vecDom] mySparseC.x(2)(j) = 2 + j/10.0;

[i in myEnumC.x.domain] [j in vecDom] myEnumC.x(i)(j) = i:int + j:real/10.0;

// check class instances

foo(myArithC);
foo(myAssocC);
foo(myOpaqueC);
foo(mySparseC);
foo(myEnumC);

delete myArithC;
delete myAssocC;
delete myOpaqueC;
delete mySparseC;
delete myEnumC;

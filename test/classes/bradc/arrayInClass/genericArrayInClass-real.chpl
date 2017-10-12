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

var myArithC = new ArithC(real);
var myAssocC = new AssocC(real);
var myOpaqueC = new OpaqueC(real);
var mySparseC = new SparseC(real);
var myEnumC = new EnumC(real);

proc deinit() {
  delete myEnumC;
  delete mySparseC;
  delete myOpaqueC;
  delete myAssocC;
  delete myArithC;
}

// initialize class instances

[i in myArithC.x.domain] myArithC.x(i) = i + i/10.0;

myAssocC.assocDom += "two";
myAssocC.x("two") = 2.1;

const newInd = myOpaqueC.opaqueDom.create();
myOpaqueC.x(newInd) = 2.2;

mySparseC.sparseDom += 2;
mySparseC.x(2) = 2.3;

[i in myEnumC.x.domain] myEnumC.x(i) = i:int + i:real/10.0;

// check class instances

foo(myArithC);
foo(myAssocC);
foo(myOpaqueC);
foo(mySparseC);
foo(myEnumC);

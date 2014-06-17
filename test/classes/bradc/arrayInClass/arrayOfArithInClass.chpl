// declare class types

class ArithC {
  var x: [1..3] [1..3] real;
}

class AssocC {
  var assocDom: domain(string);
  var x: [assocDom] [1..3] real;
}

class OpaqueC {
  var opaqueDom: domain(opaque);
  var x: [opaqueDom] [1..3] real;
}

class SparseC {
  var sparseDom: sparse subdomain({1..3});
  var x: [sparseDom] [1..3] real;
}

enum probClass {S, W, A, B, C};

class EnumC {
  var enumDom: domain(probClass);
  var x: [enumDom] [1..3] real;
}

// generic print routine

proc foo(C) {
  writeln("C.x.domain is: ", C.x.domain);
  writeln("x is: ", C.x);
  writeln();
}

// declare class instances

var myArithC = new ArithC();
var myAssocC = new AssocC();
var myOpaqueC = new OpaqueC();
var mySparseC = new SparseC();
var myEnumC = new EnumC();

// initialize class instances

[i in myArithC.x.domain] [j in 1..3] myArithC.x(i)(j) = i + j/10.0;

myAssocC.assocDom += "two";
[j in 1..3] myAssocC.x("two")(j) = 2 + j/10.0;

const newInd = myOpaqueC.opaqueDom.create();
[j in 1..3] myOpaqueC.x(newInd)(j) = 2 + j/10.0;

mySparseC.sparseDom += 2;
[j in 1..3] mySparseC.x(2)(j) = 2 + j/10.0;

[i in myEnumC.x.domain] [j in 1..3] myEnumC.x(i)(j) = i:int + j:real/10.0;

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


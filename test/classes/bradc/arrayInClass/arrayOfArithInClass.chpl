// declare class types

class ArithC {
  var x: [1..3] [1..3] real;
}

class AssocC {
  var assocDom: domain(string);
  var x: [assocDom] [1..3] real;
}

class SparseC {
  var sparseDom: sparse subdomain({1..3});
  var x: [sparseDom] [1..3] real;
}

enum probClass {S=1, W, A, B, C};

class EnumC {
  var enumDom: domain(probClass) = probClass.S..probClass.C;
  var x: [enumDom] [1..3] real;
}

// generic print routine

proc foo(C) {
  writeln("C.x.domain is: ", C.x.domain);
  writeln("x is: ", C.x);
  writeln();
}

// declare class instances

var myArithC = new unmanaged ArithC();
var myAssocC = new unmanaged AssocC();
var mySparseC = new unmanaged SparseC();
var myEnumC = new unmanaged EnumC();

// initialize class instances

[i in myArithC.x.domain] [j in 1..3] myArithC.x(i)(j) = i + j/10.0;

myAssocC.assocDom += "two";
[j in 1..3] myAssocC.x("two")(j) = 2 + j/10.0;

mySparseC.sparseDom += 2;
[j in 1..3] mySparseC.x(2)(j) = 2 + j/10.0;

[i in myEnumC.x.domain] [j in 1..3] myEnumC.x(i)(j) = i:int + j:real/10.0;

// check class instances

foo(myArithC);
foo(myAssocC);
foo(mySparseC);
foo(myEnumC);

delete myArithC;
delete myAssocC;
delete mySparseC;
delete myEnumC;


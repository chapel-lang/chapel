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

{
  // declare associative class instances

  var assocDom: domain(string);
  type assocArr = [assocDom] real;

  var myArithC = new ArithC(assocArr);
  var myAssocC = new AssocC(assocArr);
  var myOpaqueC = new OpaqueC(assocArr);
  var mySparseC = new SparseC(assocArr);
  var myEnumC = new EnumC(assocArr);

  assocDom += "two";

  // initialize class instances

  [i in myArithC.x.domain] [j in assocDom] myArithC.x(i)(j) = i + 2/10.0;
  
  myAssocC.assocDom += "two";
  [j in assocDom] myAssocC.x("two")(j) = 2 + 2/10.0;

  const newInd = myOpaqueC.opaqueDom.create();
  [j in assocDom] myOpaqueC.x(newInd)(j) = 2 + 2/10.0;

  mySparseC.sparseDom += 2;
  [j in assocDom] mySparseC.x(2)(j) = 2 + 2/10.0;

  [i in myEnumC.x.domain] [j in assocDom] myEnumC.x(i)(j) = i:int + 2:real/10.0;

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
}


{
  // declare associative class instances

  var opaqueDom: domain(opaque);
  type opaqueArr = [opaqueDom] real;

  var myArithC = new ArithC(opaqueArr);
  var myAssocC = new AssocC(opaqueArr);
  var myOpaqueC = new OpaqueC(opaqueArr);
  var mySparseC = new SparseC(opaqueArr);
  var myEnumC = new EnumC(opaqueArr);

  opaqueDom.create();

  // initialize class instances

  [i in myArithC.x.domain] [j in opaqueDom] myArithC.x(i)(j) = i + 2/10.0;
  
  myAssocC.assocDom += "two";
  [j in opaqueDom] myAssocC.x("two")(j) = 2 + 2/10.0;
  
  const newInd = myOpaqueC.opaqueDom.create();
  [j in opaqueDom] myOpaqueC.x(newInd)(j) = 2 + 2/10.0;
  
  mySparseC.sparseDom += 2;
  [j in opaqueDom] mySparseC.x(2)(j) = 2 + 2/10.0;
  
  [i in myEnumC.x.domain] [j in opaqueDom] myEnumC.x(i)(j) = i:int + 2:real/10.0;
  
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
}


{
  // declare associative class instances

  var sparseDom: sparse subdomain({1..3});
  type sparseArr = [sparseDom] real;

  var myArithC = new ArithC(sparseArr);
  var myAssocC = new AssocC(sparseArr);
  var myOpaqueC = new OpaqueC(sparseArr);
  var mySparseC = new SparseC(sparseArr);
  var myEnumC = new EnumC(sparseArr);

  sparseDom += 2;

  // initialize class instances

  [i in myArithC.x.domain] [j in sparseDom] myArithC.x(i)(j) = i + j/10.0;
  
  myAssocC.assocDom += "two";
  [j in sparseDom] myAssocC.x("two")(j) = 2 + j/10.0;
  
  const newInd = myOpaqueC.opaqueDom.create();
  [j in sparseDom] myOpaqueC.x(newInd)(j) = 2 + j/10.0;
  
  mySparseC.sparseDom += 2;
  [j in sparseDom] mySparseC.x(2)(j) = 2 + j/10.0;
  
  [i in myEnumC.x.domain] [j in sparseDom] myEnumC.x(i)(j) = i:int + j:real/10.0;
  
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
}



{
  // declare associative class instances

  var enumDom: domain(probClass);
  type enumArr = [enumDom] real;

  var myArithC = new ArithC(enumArr);
  var myAssocC = new AssocC(enumArr);
  var myOpaqueC = new OpaqueC(enumArr);
  var mySparseC = new SparseC(enumArr);
  var myEnumC = new EnumC(enumArr);

  // initialize class instances

  [i in myArithC.x.domain] [j in enumDom] myArithC.x(i)(j) = i + j:real/10.0;
  
  myAssocC.assocDom += "two";
  [j in enumDom] myAssocC.x("two")(j) = 2 + j:real/10.0;
  
  const newInd = myOpaqueC.opaqueDom.create();
  [j in enumDom] myOpaqueC.x(newInd)(j) = 2 + j:real/10.0;
  
  mySparseC.sparseDom += 2;
  [j in enumDom] mySparseC.x(2)(j) = 2 + j:real/10.0;
  
  [i in myEnumC.x.domain] [j in enumDom] myEnumC.x(i)(j) = i:int + j:real/10.0;
  
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
}

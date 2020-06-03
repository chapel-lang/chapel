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

class SparseC {
  type t;
  
  var sparseDom: sparse subdomain({1..3});
  var x: [sparseDom] t;
}

enum probClass {S=1, W, A, B, C};

class EnumC {
  type t;
  
  var enumDom: domain(probClass) = probClass.S..probClass.C;
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

  var myArithC = new unmanaged ArithC(assocArr);
  var myAssocC = new unmanaged AssocC(assocArr);
  var mySparseC = new unmanaged SparseC(assocArr);
  var myEnumC = new unmanaged EnumC(assocArr);

  assocDom += "two";

  // initialize class instances

  [i in myArithC.x.domain] [j in assocDom] myArithC.x(i)(j) = i + 2/10.0;
  
  myAssocC.assocDom += "two";
  [j in assocDom] myAssocC.x("two")(j) = 2 + 2/10.0;

  mySparseC.sparseDom += 2;
  [j in assocDom] mySparseC.x(2)(j) = 2 + 2/10.0;

  [i in myEnumC.x.domain] [j in assocDom] myEnumC.x(i)(j) = i:int + 2:real/10.0;

  // check class instances

  foo(myArithC);
  foo(myAssocC);
  foo(mySparseC);
  foo(myEnumC);

  delete myArithC;
  delete myAssocC;
  delete mySparseC;
  delete myEnumC;
}


{
  // declare associative class instances

  var sparseDom: sparse subdomain({1..3});
  type sparseArr = [sparseDom] real;

  var myArithC = new unmanaged ArithC(sparseArr);
  var myAssocC = new unmanaged AssocC(sparseArr);
  var mySparseC = new unmanaged SparseC(sparseArr);
  var myEnumC = new unmanaged EnumC(sparseArr);

  sparseDom += 2;

  // initialize class instances

  [i in myArithC.x.domain] [j in sparseDom] myArithC.x(i)(j) = i + j/10.0;
  
  myAssocC.assocDom += "two";
  [j in sparseDom] myAssocC.x("two")(j) = 2 + j/10.0;
  
  mySparseC.sparseDom += 2;
  [j in sparseDom] mySparseC.x(2)(j) = 2 + j/10.0;
  
  [i in myEnumC.x.domain] [j in sparseDom] myEnumC.x(i)(j) = i:int + j:real/10.0;
  
  // check class instances
  
  foo(myArithC);
  foo(myAssocC);
  foo(mySparseC);
  foo(myEnumC);

  delete myArithC;
  delete myAssocC;
  delete mySparseC;
  delete myEnumC;
}



{
  // declare associative class instances

  var enumDom: domain(probClass) = probClass.S..probClass.C;
  type enumArr = [enumDom] real;

  var myArithC = new unmanaged ArithC(enumArr);
  var myAssocC = new unmanaged AssocC(enumArr);
  var mySparseC = new unmanaged SparseC(enumArr);
  var myEnumC = new unmanaged EnumC(enumArr);

  // initialize class instances

  [i in myArithC.x.domain] [j in enumDom] myArithC.x(i)(j) = i + j:real/10.0;
  
  myAssocC.assocDom += "two";
  [j in enumDom] myAssocC.x("two")(j) = 2 + j:real/10.0;
  
  mySparseC.sparseDom += 2;
  [j in enumDom] mySparseC.x(2)(j) = 2 + j:real/10.0;
  
  [i in myEnumC.x.domain] [j in enumDom] myEnumC.x(i)(j) = i:int + j:real/10.0;
  
  // check class instances
  
  foo(myArithC);
  foo(myAssocC);
  foo(mySparseC);
  foo(myEnumC);

  delete myArithC;
  delete myAssocC;
  delete mySparseC;
  delete myEnumC;
}

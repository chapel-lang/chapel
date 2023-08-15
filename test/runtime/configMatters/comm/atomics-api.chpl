use AtomicsAPI;

proc declareAndTestAtomicBool() {
  var a, a2: atomic bool;
  var i: bool;
  var b: bool;
  testAtomicBool(a, i, b);
  testOrderAtomicBool(a2, i, b, memoryOrder.seqCst);
}

proc declareAndTestAtomicT(type valType) {
  var a, a2: atomic valType;
  var i: valType;
  var b: bool;
  testAtomicT(a, i, b);
  testOrderAtomicT(a2, i, b, memoryOrder.seqCst);
}

/* Test full API for all types */
declareAndTestAtomicBool();

declareAndTestAtomicT(int(8));
declareAndTestAtomicT(int(16));
declareAndTestAtomicT(int(32));
declareAndTestAtomicT(int(64));
declareAndTestAtomicT(int);

declareAndTestAtomicT(uint(8));
declareAndTestAtomicT(uint(16));
declareAndTestAtomicT(uint(32));
declareAndTestAtomicT(uint(64));
declareAndTestAtomicT(uint);

declareAndTestAtomicT(real(32));
declareAndTestAtomicT(real(64));
declareAndTestAtomicT(real);

/* Test promotion for atomic int */
var AInt, AInt2: [1..3] atomic int;
var IInt: [1..3] int;
var BInt: [1..3] bool;
write("Promotion -- ");
testAtomicT(AInt, IInt, BInt);
testOrderAtomicT(AInt2, IInt, BInt, memoryOrder.seqCst);

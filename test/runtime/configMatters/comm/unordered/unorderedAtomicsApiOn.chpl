use AtomicsAPI;

config const lastLocale = false;
const onLocale = if lastLocale then numLocales-1 else 0;

proc declareAndTestAtomicT(type basetype) {
  var a: atomic basetype;
  var i: basetype;
  var b: bool;
  on Locales[onLocale] {
    testUnorderedAtomicT(a, i, b, basetype);
  }
}

/* Test full API for all integer and real types */
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
var AInt: [1..3] atomic int;
var IInt: [1..3] int;
var BInt: [1..3] bool;
on Locales[onLocale] {
  write("Promotion -- ");
  testUnorderedAtomicT(AInt, IInt, BInt, int);
}

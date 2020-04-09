// This is a "minimal reproducer" for a valgrind issue
// that workaroundForReduceIntoDetupleDecl() deals with.
// The full versions are:
//   test/studies/kmeans/kmeansonepassreduction-minchange.chpl
//   test/studies/kmeans/kmeans-blc.chpl
//
// While this reproducer does not add testing coverage,
// I am adding it to facilitate resolution of #12497
// and just in case the other two tests are modified
// and do not exhibit the problematic pattern any longer.

var GLOBAL: [1..3] int;

// Returns a tuple with an array.
proc thisWorks() {
  return (GLOBAL,);
}

proc useme(arg) {
  writeln(arg);
}

iter dataIter() {
  yield 55555;
}

// The role of this reduce-op class is to illustrate a generate() function
// that returns a tuple with an array.
class kmeans: ReduceScanOp {
  type eltType;
  proc identity {  return 333333; }
  proc accumulateOntoState(ref state, datum) {}
  proc accumulate(value) {}
  proc combine(other: borrowed kmeans) {}
  proc generate() { return (GLOBAL,);  }
  proc clone() {   return new unmanaged kmeans(eltType=eltType);  }
}

proc main {
  writeln("START MAIN -----------");
  var (offsetsR,) = thisWorks();
  writeln("DIVIDER 1  -----------");
  useme((...thisWorks()));
  writeln("DIVIDER 2  -----------");
  // This is the case that requires workaroundForReduceIntoDetupleDecl().
  var (offsetsI,) = kmeans reduce dataIter();
  writeln("DONE MAIN  -----------");
}

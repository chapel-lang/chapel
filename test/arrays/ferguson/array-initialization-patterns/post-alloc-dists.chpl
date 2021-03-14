// This test uses debug output from DefaultRectangular
// to show allocations, post-allocation, and deallocation.

// If the output changes, just make sure:
//   * the number of allocations matches the number of deallocations
//   * the number of allocations is reasonable/expected
//   * postalloc is being called

use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist, StencilDist;

enum DistType { default, block, cyclic, blockcyclic, replicated, stencil };
config param distType: DistType = DistType.block;

writeln("alloc int test begins");

proc makeA() {
  if distType == DistType.default {
    var ret: [1..1] int;
    return ret;
  } else if distType == DistType.block {
    return newBlockArr(1..1, int);
  } else if distType == DistType.cyclic {
    return newCyclicArr(1..1, int);
  } else if distType == DistType.blockcyclic {
    var D = {1..1} dmapped BlockCyclic(startIdx=(1,), (3,));
    var ret: [D] int;
    return ret;
  } else if distType == DistType.replicated {
    var D = {1..1} dmapped Replicated();
    var ret: [D] int;
    return ret;
  } else if distType == DistType.stencil {
    var D = {1..1} dmapped Stencil(rank=1, boundingBox={1..1});
    var ret: [D] int;
    return ret;
  } else {
    compilationError("unknown case");
  }
}

proc test0() {
  writeln("alloc int test0");
  var A = makeA();
}
test0();

proc test1() {
  writeln("alloc int test1");
  var A = makeA();
  var B:A.type;
}
test1();

proc test2() {
  writeln("alloc int test2");
  var A = makeA();
  var B = A; // copy elided
}
test2();

proc test3() {
  writeln("alloc int test3");
  var A = makeA();
  var B:A.type = A; // copy elided
}
test3();

proc test4() {
  writeln("alloc int test4");
  var A = makeA();
  var B = A; // no copy elision
  writeln(A);
}
test4();

proc test5() {
  writeln("alloc int test5");
  var A = makeA();
  var B:A.type = A; // no copy elision
  writeln(A);
}
test5();

writeln("alloc int test ends");

// This test shows several variants a pattern in
//   test/studies/dijkstra/driverEdgeBtw.chpl
// that was leaking memory.

proc InitializeEdgesOut(nEdges: int, out nNodes: int) {
  var nodes: int;

  var D1 = {0..(nEdges-1)};
  var Edges: [D1] int;
  nNodes = nodes;
  return Edges;
}

proc test1() {
  writeln("test1");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges = InitializeEdgesOut(nEdges, nNodes);
  writeln(Edges);
}
test1();

proc test1t() {
  writeln("test1t");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges: [0..nEdges-1] int = InitializeEdgesOut(nEdges, nNodes);
  writeln(Edges);
}
test1t();

proc test2() {
  writeln("test2");
  var nEdges: int = 1;
  var nNodes; // split init

  var Edges = InitializeEdgesOut(nEdges, nNodes);
  writeln(Edges);
}
test2();

proc test2t() {
  writeln("test2t");
  var nEdges: int = 1;
  var nNodes; // split init

  var Edges: [0..nEdges-1] int = InitializeEdgesOut(nEdges, nNodes);
  writeln(Edges);
}
test2t();

proc InitializeEdgesInout(nEdges: int, inout nNodes: int) {
  var nodes: int;

  var D1 = {0..(nEdges-1)};
  var Edges: [D1] int;
  nNodes = nodes;
  return Edges;
}

proc test3() {
  writeln("test3");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges = InitializeEdgesInout(nEdges, nNodes);
  writeln(Edges);
}
test3();

proc test3t() {
  writeln("test3t");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges: [0..nEdges-1] int = InitializeEdgesInout(nEdges, nNodes);
  writeln(Edges);
}
test3();

proc test4() {
  writeln("test4");
  var nEdges: int = 1;
  var nNodes: int;

  var Edges = InitializeEdgesInout(nEdges, nNodes);
  writeln(Edges);
}
test4();

proc test4t() {
  writeln("test4t");
  var nEdges: int = 1;
  var nNodes: int;

  var Edges: [0..nEdges-1] int = InitializeEdgesInout(nEdges, nNodes);
  writeln(Edges);
}
test4t();

config const printInitDeinitR = false;

class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    if printInitDeinitR then writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    if printInitDeinitR then writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    if printInitDeinitR then writeln("init= ", other.x);
  }
  proc deinit() {
    if printInitDeinitR then writeln("deinit ", this.x);
  }
}
operator R.=(ref lhs:R, rhs:R) {
  if printInitDeinitR then writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

proc InitializeRecordOut(nEdges: int, out nNodes: int) {
  var nodes: int;

  var Edges = new R(nEdges);
  nNodes = nodes;
  return Edges;
}

proc test5() {
  writeln("test5");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges = InitializeRecordOut(nEdges, nNodes);
  writeln(Edges.x);
}
test5();

proc test5t() {
  writeln("test5t");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges: R = InitializeRecordOut(nEdges, nNodes);
  writeln(Edges.x);
}
test5();

proc test6() {
  writeln("test6");
  var nEdges: int = 1;
  var nNodes; // split init

  var Edges = InitializeRecordOut(nEdges, nNodes);
  writeln(Edges.x);
}
test6();

proc test6t() {
  writeln("test6t");
  var nEdges: int = 1;
  var nNodes; // split init

  var Edges: R;
  Edges = InitializeRecordOut(nEdges, nNodes);
  writeln(Edges.x);
}
test6t();

proc InitializeRecordInout(nEdges: int, inout nNodes: int) {
  var nodes: int;

  var Edges = new R(nEdges);
  nNodes = nodes;
  return Edges;
}

proc test7() {
  writeln("test7");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges = InitializeRecordInout(nEdges, nNodes);
  writeln(Edges.x);
}
test7();

proc test7t() {
  writeln("test7t");
  var nEdges: int = 1;
  var nNodes: int = 0;

  var Edges: R = InitializeRecordInout(nEdges, nNodes);
  writeln(Edges.x);
}
test7t();

proc test8() {
  writeln("test8");
  var nEdges: int = 1;
  var nNodes: int;

  var Edges = InitializeRecordInout(nEdges, nNodes);
  writeln(Edges.x);
}
test8();

proc test8t() {
  writeln("test8t");
  var nEdges: int = 1;
  var nNodes: int;

  var Edges: R;
  Edges = InitializeRecordInout(nEdges, nNodes);
  writeln(Edges.x);
}
test8t();



use BlockDist;
use CyclicDist;

proc testBlockDom() {
  var D = {0..9};
  var D4 = D dmapped Block;

  writeln("testBlockDom ", D4.type:string);

  forall a in D4 {
    writeln("testBlockDom ", a, " on ", here.id);
  }
}

testBlockDom();

proc testCyclicArr() {
  var A1 : [{0..9} dmapped Cyclic] int = 0..9;

  writeln("testCyclicArr ", A1.domain.type:string);

  forall a in A1.domain {
    writeln("testCyclicArr ", a, " on ", here.id);
  }
}

testCyclicArr();

proc testCyclicDom() {
  var D = {0..9};
  var D4 = D dmapped Cyclic;

  writeln("testCyclicDom ", D4.type:string);

  forall a in D4 {
    writeln("testCyclicDom ", a, " on ", here.id);
  }
}

testCyclicDom();

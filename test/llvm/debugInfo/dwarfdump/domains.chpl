use Debugger;
use BlockDist;
use CyclicDist;
use BlockCycDist;

// DWARFDUMP: DefaultDist
// DWARFDUMP: BaseDist


proc testDR() {

  // DWARFDUMP: myDomain1D
  // DWARFDUMP: domain(1,int(64),one)
  // DWARFDUMP: BaseRectangularDom(1,int(64),one)
  var myDomain1D = {1..10};
  // DWARFDUMP: myDomain2D
  // DWARFDUMP: domain(2,int(64),one)
  // DWARFDUMP: BaseRectangularDom(2,int(64),one)
  const myDomain2D = {1..10, 1..10};
  // DWARFDUMP: myStridedDom
  // DWARFDUMP: domain(1,int(64),positive)
  // DWARFDUMP: BaseRectangularDom(1,int(64),positive)
  var myStridedDom = {1..10 by 2};
  // DWARFDUMP: myReverseDom
  // DWARFDUMP: domain(1,int(64),negOne)
  // DWARFDUMP: BaseRectangularDom(1,int(64),negOne)
  const myReverseDom = {1..#20 by -1};
  // DWARFDUMP: myEmptyDom
  const myEmptyDom = {1..0};
  // DWARFDUMP: myUintDom
  // DWARFDUMP: domain(1,uint(32),one)
  // DWARFDUMP: BaseRectangularDom(1,uint(32),one)
  var myUintDom = {0:uint(32)..15:uint(32)};

  writeln(myDomain1D, myDomain2D, myStridedDom, myReverseDom, myEmptyDom, myUintDom, sep=" | ");
  breakpoint;
}
proc testAssoc() {

  // DWARFDUMP: myAssocDom
  var myAssocDom = {1, 3, 5, 7, 9};
  // DWARFDUMP: myAssocStringDom
  var myAssocStringDom = {"apple", "banana", "cherry"};

  writeln(myAssocDom, myAssocStringDom, sep=" | ");
  breakpoint;
}
proc testSparse() {
  var baseDom = {1..10};
  // DWARFDUMP: mySparseDom
  // DWARFDUMP: DefaultSparseDom(1,int(64),domain(1,int(64),one))
  var mySparseDom: sparse subdomain(baseDom);
  mySparseDom += 1;
  mySparseDom += 9;

  writeln(mySparseDom);
  breakpoint;
}
proc testBlockDist() {

  // DWARFDUMP: myBlockDist
  // DWARFDUMP: BlockDom(1,int(64),one,unmanaged DefaultDist)
  // DWARFDUMP: LocBlockDom(1,int(64),one)
  // DWARFDUMP: BlockImpl(1,int(64),unmanaged DefaultDist)
  var myBlockDist = blockDist.createDomain(1..100);
  // DWARFDUMP: myBlockDist2D
  // DWARFDUMP: BlockDom(2,int(64),positive,unmanaged DefaultDist)
  // DWARFDUMP: LocBlockDom(2,int(64),positive)
  // DWARFDUMP: BlockImpl(2,int(64),unmanaged DefaultDist)
  // DWARFDUMP: LocBlock(2,int(64))
  var myBlockDist2D = blockDist.createDomain({1..10 by 2, 1..10});
  // DWARFDUMP: myBlockDistInt16
  // DWARFDUMP: BlockDom(1,int(16),one,unmanaged DefaultDist)
  // DWARFDUMP: LocBlockDom(1,int(16),one)
  // DWARFDUMP: BlockImpl(1,int(16),unmanaged DefaultDist)
  var myBlockDistInt16 = blockDist.createDomain({-5:int(16)..15:int(16)});

  writeln(myBlockDist, myBlockDist2D, myBlockDistInt16, sep=" | ");
  breakpoint;
}
proc testCyclicDist() {

  // DWARFDUMP: myCyclicDist
  // DWARFDUMP: CyclicDom(1,int(64),one)
  // DWARFDUMP: LocCyclicDom(1,int(64))
  // DWARFDUMO: CyclicImpl(1,int(64))
  var myCyclicDist = cyclicDist.createDomain(1..100);
  // DWARFDUMP: myCyclicDist2D
  // DWARFDUMP: CyclicDom(2,int(64),positive)
  // DWARFDUMP: LocCyclicDom(2,int(64))
  // DWARFDUMO: CyclicImpl(2,int(64))
  var myCyclicDist2D = cyclicDist.createDomain({1..10 by 2, 1..10});
  // DWARFDUMP: myCyclicDistUint
  // DWARFDUMP: CyclicDom(1,uint(32),one)
  // DWARFDUMP: LocCyclicDom(1,uint(32))
  // DWARFDUMO: CyclicImpl(1,uint(32))
  var myCyclicDistUint = cyclicDist.createDomain({5:uint(32)..15:uint(32)});

  writeln(myCyclicDist, myCyclicDist2D, myCyclicDistUint, sep=" | ");
  breakpoint;
}
proc testBlockCyclicDist() {

  // DWARFDUMP: myBlockCyclicDist
  // DWARFDUMP: BlockCyclicDom(1,int(64),one)
  // DWARFDUMP: LocBlockCyclicDom(1,int(64),one)
  // DWARFDUMP: BlockCyclicImpl(1,int(64))
  var myBlockCyclicDist = {1..100} dmapped new blockCycDist(1, 4);
  // DWARFDUMP: myBlockCyclicDist2D
  // DWARFDUMP: BlockCyclicDom(2,int(64),positive)
  // DWARFDUMP: LocBlockCyclicDom(2,int(64),positive)
  // DWARFDUMP: BlockCyclicImpl(2,int(64))
  var myBlockCyclicDist2D = {1..10 by 2, 1..10} dmapped new blockCycDist((1,1), (2,2));

  writeln(myBlockCyclicDist, myBlockCyclicDist2D, sep=" | ");
  breakpoint;
}

proc main() {
  testDR();
  testAssoc();
  testSparse();
  testBlockDist();
  testCyclicDist();
  testBlockCyclicDist();
}

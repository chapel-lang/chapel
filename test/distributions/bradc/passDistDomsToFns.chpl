use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist;
use DimensionalDist2D, ReplicatedDim, BlockCycDim;

const Space = {1..8, 1..8};
const BlockSpace = Space dmapped Block(boundingBox=Space);
var BA: [BlockSpace] int;
fooB(BA);
proc fooB(X:[BlockSpace] int) {
  writeln("In fooB!");
}


const CyclicSpace = Space dmapped Cyclic(startIdx=Space.low);
var CA: [CyclicSpace] int;
fooC(CA);
proc fooC(X:[CyclicSpace] int) {
  writeln("In fooC!");
}


const BlkCycSpace = Space dmapped BlockCyclic(startIdx=Space.low, 
                                              blocksize=(2, 3));
var BCA: [BlkCycSpace] int;
fooBC(BCA);
proc fooBC(X:[BlkCycSpace] int) {
  writeln("In fooBC!");
}


const ReplicatedSpace = Space dmapped Replicated();
var RA: [ReplicatedSpace] int;
fooR(RA);
proc fooR(X:[ReplicatedSpace] int) {
  writeln("In fooR!");
}


var (nl1, nl2) = if numLocales == 1 then (1, 1) else (2, numLocales/2);
var MyLocaleView = {0..#nl1, 0..#nl2};
var MyLocales = reshape(Locales[0..#nl1*nl2], MyLocaleView);

const DimReplicatedBlockcyclicSpace = Space
  dmapped DimensionalDist2D(MyLocales,
                            new ReplicatedDim(numLocales = nl1),
                            new BlockCyclicDim(numLocales = nl2,
                                               lowIdx = 1, blockSize = 2));

var DRBA: [DimReplicatedBlockcyclicSpace] int;
fooDRB(DRBA);
proc fooDRB(X:[DimReplicatedBlockcyclicSpace] int) {
  writeln("In fooDRB!");
}

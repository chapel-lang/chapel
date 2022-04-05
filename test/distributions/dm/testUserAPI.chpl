// derived from domains/userAPI/test2Ddomain.chpl

use DimensionalDist2D, BlockCycDim, BlockDim;
require "../../domains/userAPI/domainAPItest.chpl";
use domainAPItest only testDomainAPI2D as doTestDomainAPI2D;

const (s1,s2) = if numLocales < 4 then (1,1) else (2,2);
const mylocdom = {0..#s1, 0..#s2};
const mylocs = reshape(Locales, mylocdom);

const bl1 = new BlockDim(s1, 1..5);
const bl2 = new BlockDim(s2, -3..7);
const bc1 = new BlockCyclicDim(s1, 1, 2);
const bc2 = new BlockCyclicDim(s2, -1, 4);

const BlBl = new dmap(new DimensionalDist2D(mylocs, bl1, bl2, "bl1,bl2"));
const BlBc = new dmap(new DimensionalDist2D(mylocs, bl1, bc2, "bl1,bc2"));
const BcBl = new dmap(new DimensionalDist2D(mylocs, bc1, bl2, "bc1,bl2"));
const BcBc = new dmap(new DimensionalDist2D(mylocs, bc1, bc2, "bc1,bc2"));

proc testDomainAPI2D(param lbl, DR: domain, idx, OOBidx1, OOBidx2, intDom) {
  inline proc help(dd) {
    write("### ", dd.name, " ");
    doTestDomainAPI2D(lbl, DR dmapped dd, idx, OOBidx1, OOBidx2, intDom);
  }
  help(BlBl);
  help(BlBc);
  help(BcBl);
  help(BcBc);
}


proc main {

testDomainAPI2D("2D domain", {1..5, 0..4}, (3, 3), (0, 3), (3, 5), {0..6, 3..5});
testDomainAPI2D("strided 2D domain", {1..5 by 2, 0..4 by 2 align 1}, (3, 3), (0, 3), (3, 5), {0..6, 3..5});

}

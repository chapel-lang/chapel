// derived from domains/userAPI/test2Ddomain.chpl

use ReplicatedDist;
require "../../domains/userAPI/domainAPItest.chpl";
use domainAPItest only testDomainAPI2D as doTestDomainAPI2D;

proc testDomainAPI2D(param lbl, DR: domain, idx, OOBidx1, OOBidx2, intDom) {
  doTestDomainAPI2D(lbl, DR dmapped Replicated(), idx, OOBidx1, OOBidx2, intDom);
}

proc main {

testDomainAPI2D("2D domain", {1..5, 0..4}, (3, 3), (0, 3), (3, 5), {0..6, 3..5});
testDomainAPI2D("strided 2D domain", {1..5 by 2, 0..4 by 2 align 1}, (3, 3), (0, 3), (3, 5), {0..6, 3..5});

}

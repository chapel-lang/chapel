use LinearAlgebra;

use BlockDist;
use CompressedSparseLayout;


var distArr = blockDist.createArray({1..10}, int);
var sliceOfDist = distArr[1..8];
var locArr: [1..8] int;

var locSparseDom: sparse subdomain(locArr.domain);
var locSparseArr: [locSparseDom] int;

var space2D = {1..10, 1..10};
var locSparseDomCSR: sparse subdomain(space2D) dmapped new cs();
var locSparseArrCSR: [locSparseDomCSR] int;

var distSparseDom: sparse subdomain(distArr.domain);
var distSparseArr: [distSparseDom] int;

var distCSRBlock = {1..10, 1..10} dmapped new blockDist({1..10, 1..10}, sparseLayoutType=cs);
var distSparseArrCSR: [distCSRBlock] int;

assert(isDistributed(distArr) == true);
assert(isDistributed(sliceOfDist) == true);
assert(isDistributed(locArr) == false);
assert(isDistributed(locSparseArr) == false);
assert(isDistributed(locSparseArrCSR) == false);
assert(isDistributed(distSparseArr) == true);
assert(isDistributed(distSparseArrCSR) == true);

use LinearAlgebra;

use BlockDist;
use LayoutCS;


var distArr = newBlockArr({1..10}, int);
var sliceOfDist = distArr[1..8];
var locArr: [1..8] int;

var locSparseDom: sparse subdomain(locArr.domain);
var locSparseArr: [locSparseDom] int;

var space2D = {1..10, 1..10};
var locSparseDomCSR: sparse subdomain(space2D) dmapped CS();
var locSparseArrCSR: [locSparseDomCSR] int;

var distSparseDom: sparse subdomain(distArr.domain);
var distSparseArr: [distSparseDom] int;

var distCSRBlock = {1..10, 1..10} dmapped Block({1..10, 1..10}, sparseLayoutType=CS);
var distSparseArrCSR: [distCSRBlock] int;

assert(isDistributed(distArr) == true);
assert(isDistributed(sliceOfDist) == true);
assert(isDistributed(locArr) == false);
assert(isDistributed(locSparseArr) == false);
assert(isDistributed(locSparseArrCSR) == false);
assert(isDistributed(distSparseArr) == true);
assert(isDistributed(distSparseArrCSR) == true);

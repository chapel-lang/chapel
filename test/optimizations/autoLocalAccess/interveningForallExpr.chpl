use BlockDist;
use CyclicDist;

var blockArr = blockDist.createArray(1..10, int);
var cyclicDom = cyclicDist.createDomain(1..10);

var sums: [1..10] int;

forall blockIdx in blockArr.domain with (ref sums) {
  var dummy = [cyclicIdx in cyclicDom] 2*blockArr[blockIdx];
  sums[blockIdx] = + reduce dummy;
}

writeln(sums);

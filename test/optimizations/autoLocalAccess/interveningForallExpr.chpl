use BlockDist;
use CyclicDist;

var blockArr = Block.createArray(1..10, int);
var cyclicDom = Cyclic.createDomain(1..10);

var sums: [1..10] int;

forall blockIdx in blockArr.domain {
  var dummy = [cyclicIdx in cyclicDom] 2*blockArr[blockIdx];
  sums[blockIdx] = + reduce dummy;
}

writeln(sums);

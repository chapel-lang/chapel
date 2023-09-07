use BlockDist;

var blockDom = blockDist.createDomain(3..8);
var localArr: [1..10] int;

ref slice = localArr[blockDom];

forall i in slice.domain with (ref slice) {// I expect this to be a distributed
                                           // loop but this shouldn't be a
                                           // localAccess (?)
  slice[i] += 1;
}

writeln(localArr);

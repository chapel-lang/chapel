use BlockDist;

var blockDom = Block.createDomain(3..8);
var localArr: [1..10] int;

ref slice = localArr[blockDom];

forall i in slice.domain {        // I expect this to be a distributed loop
  slice[i] += 1;                  // but this shouldn't be a localAccess (?)
}

writeln(localArr);

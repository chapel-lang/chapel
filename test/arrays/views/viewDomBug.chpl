use BlockDist;

var blockDom = newBlockDom(3..8);
var localArr: [1..10] int;

ref slice = localArr[blockDom];

forall i in slice.domain {        // I expect this to be a distributed loop
  slice[i] += 1;                  // but this shouldn't be a localAccess (?)
}

writeln(localArr);


var blockDom2 = {3..8 by 2} dmapped Block({3..8});
ref slice2 = localArr[blockDom2];

forall i in slice2.domain {
  slice2[i] += 1;
}

writeln(localArr);


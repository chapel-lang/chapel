use BlockDist;

var arr = blockDist.createArray(1..10, int);

ref localSlice = arr[{3..8}];

forall i in localSlice.domain {
  localSlice[i] = i;
}

writeln(arr);

use BlockDist;


var Arr = blockDist.createArray(1..10, int);

const Inner = Arr.domain.expand(-1);

forall i in Arr.domain { // this should make Arr[i] a static candidate
  if i<10 then
    Arr[i] =  // this must be a local access (9 total)
      Arr[i+1];  // this must be a default access (9 total)
}

writeln(Arr);

forall i in Inner { // this should make Arr[i] a dynamic candidate
    Arr[i] =  // this must be a local access (8 total) <- this was buggy
      Arr[i+1];  // this must be a default access (8 total)
}

writeln(Arr);

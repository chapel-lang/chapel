use BlockDist;
var BaseArr = blockDist.createArray({1..2, 1..7}, int);

ref RankChangeViewLocal = BaseArr[1, ..];
ref RankChangeViewRemote = BaseArr[2, ..];

const Dom = {1..7};

forall i in Dom {
  RankChangeViewLocal[i] = i;
}

// this should fail the dynamic check
forall i in Dom {
  RankChangeViewRemote[i] = i;
}

writeln(BaseArr);

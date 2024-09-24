use BlockDist;
var BaseArr = blockDist.createArray({1..2, 1..7}, int);

ref SliceViewLocal = BaseArr[1..1, ..];
ref SliceViewRemote = BaseArr[2..2, ..];

const LocalsDom = {1..1, 1..7};
const RemotesDom = {2..2, 1..7};

forall idx in LocalsDom {
  SliceViewLocal[idx] = 1;
}

// this should fail the dynamic check
forall idx in RemotesDom {
  SliceViewRemote[idx] = 2;
}

writeln(BaseArr);

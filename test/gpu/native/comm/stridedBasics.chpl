
proc test(Whole, Slice) {
  param hostNum = 5;
  param devNum = 1;
  var HostArr: [Whole] int = hostNum;

  var rngs : Slice.rank*range;
  for param i in 0..<Slice.rank do
    rngs(i) = 1..#Slice.dim(i).size;

  writeln("--------------------");
  writeln("Whole = ", Whole);
  writeln("Slice = ", Slice);

  on here.gpus[0] {
    var DevArr: [{(...rngs)}] int;

    DevArr = HostArr[Slice];
    forall d in DevArr do assert(d == hostNum);

    @assertOnGpu
    foreach d in DevArr do d = devNum;

    HostArr[Slice] = DevArr;
  }

  forall i in Whole {
    if Slice.contains(i) then assert(HostArr[i] == devNum);
    else assert(HostArr[i] == hostNum);
  }

  writeln("SUCCESS");
  writeln("--------------------");
}

// Test stride levels 1-4
test({1..10, 1..10}, {1..10, 1..2});
test({1..10, 1..10, 1..10}, {1..10 by 3, 1..10 by 2, 1..4});
test({1..10, 1..10, 1..10, 1..10}, {1..10 by 2, 1..10 by 3, 1..10 by 2, 1..4});
test({1..10, 1..10, 1..10, 1..10, 1..10}, {1..10 by 3, 1..10 by 2, 1..10 by 3, 1..10 by 2, 1..4});

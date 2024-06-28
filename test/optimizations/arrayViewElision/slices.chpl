config param useDomain = true;
config param rank = 1;

proc multuplify(param rank, x) {
  var ret: rank*x.type;
  for param i in 0..<rank {
    ret[i] = x;
  }
  return ret;
}

var arrRanges = multuplify(rank, 1..5);

var A: [(...arrRanges)] int = 1;
var B: [(...arrRanges)] int = 2;

var counter = 1;
proc test(dstRange, srcRange) {
  const dstSlice = multuplify(rank, dstRange);
  const srcSlice = multuplify(rank, dstRange);

  if useDomain {
    A[{(...dstSlice)}] = B[{(...srcSlice)}];
  }
  else {
    A[(...dstSlice)] = B[(...srcSlice)];
  }

  writeln("Test ", counter);
  writeln(A);
  writeln();
  counter += 1;

  A = 1; // reset
}

writeln("Set first two:");
test(1..2, 1..2);
test(1..2, 4..5);
if !useDomain {
  test(1..2, ..2);
  test(1..2, 4..);
  test(..2, 1..2);
  test(..2, 3..4);
  test(..2, ..2);
  test(..2, 4..);
}
writeln("-----------------");

writeln("Set last two:");
test(4..5, 1..2);
test(4..5, 4..5);
if !useDomain {
  test(4..5, ..2);
  test(4..5, 4..);
  test(4.., 1..2);
  test(4.., 4..5);
  test(4.., ..2);
  test(4.., 4..);
}
writeln("-----------------");

writeln("Set all:");
test(1..5, 1..5);
test(1..5, 1..);
if !useDomain {
  test(1..5, ..);
  test(1.., 1..5);
  test(1.., 1..);
  test(1.., ..);
  test(.., 1..5);
  test(.., 1..);
  test(.., ..);
}
writeln("-----------------");

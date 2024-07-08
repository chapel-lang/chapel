config param useDomain = true, rank = 1;

proc multuplify(param rank, x) {
  var ret: rank*x.type;
  for param i in 0..<rank {
    ret[i] = x;
  }
  return ret;
}

var arrRanges = multuplify(rank, 1..5);

param aVal = 1, bVal = 2;

var A: [(...arrRanges)] int = aVal;
var B: [(...arrRanges)] int = bVal;

var counter = 1;
proc test(dstRange, srcRange) {
  const dstSlice = multuplify(rank, dstRange);
  const srcSlice = multuplify(rank, dstRange);

  if useDomain then
    A[{(...dstSlice)}] = B[{(...srcSlice)}];
  else
    A[(...dstSlice)] = B[(...srcSlice)];

  const checkDom = A.domain[(...dstSlice)];

  writeln("Test ", counter);
  for idx in A.domain {
    const checkVal = if checkDom.contains(idx) then bVal else aVal;
    assert(A[idx] == checkVal);
  }

  writeln(A);
  writeln();
  counter += 1;

  A = aVal; // reset
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

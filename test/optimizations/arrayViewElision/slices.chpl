config param useDomain = false, rank = 1;
config param dist = "stencil";

proc multuplify(param rank, x) {
  var ret: rank*x.type;
  for param i in 0..<rank {
    ret[i] = x;
  }
  return ret;
}

var arrRanges = multuplify(rank, 1..5);

param aVal = 1, bVal = 2;

proc createArrays() {
  select dist {
    when "default" {
      var A: [(...arrRanges)] int = aVal;
      var B: [(...arrRanges)] int = bVal;
      A = aVal;
      B = bVal;
      return (A, B);
    }
    when "block" {
      use BlockDist;
      var A = blockDist.createArray((...arrRanges), int);
      var B = blockDist.createArray((...arrRanges), int);
      A = aVal;
      B = bVal;
      return (A, B);
    }
    when "cyclic" {
      use CyclicDist;
      var A = cyclicDist.createArray((...arrRanges), int);
      var B = cyclicDist.createArray((...arrRanges), int);
      A = aVal;
      B = bVal;
      return (A, B);
    }
    when "stencil" {
      use StencilDist;
      var A = stencilDist.createArray((...arrRanges), int);
      var B = stencilDist.createArray((...arrRanges), int);
      A = aVal;
      B = bVal;
      return (A, B);
    }
  }
}

var (A, B) = createArrays();


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
  writeln(A);
  writeln();

  for idx in A.domain {
    const checkVal = if checkDom.contains(idx) then bVal else aVal;
    assert(A[idx] == checkVal);
  }

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

config const verbose = false;

config const n = 2 * defRectArrMultiDDataSizeThreshold;

const baseVal = 12345;

testRank(1, {1..n});
testRank(2, {1..ceil(exp(log(n)/2)):int,
             1..ceil(exp(log(n)/2)):int});
testRank(3, {1..ceil(exp(log(n)/3)):int,
             1..ceil(exp(log(n)/3)):int,
             1..ceil(exp(log(n)/3)):int});

proc testRank(r, D) {
  if verbose then
    writeln('Rank ', r, ': ', D);

  var A1: [D] int;
  var A2: [D] int;
  var B: [D] int;

  var iVal = baseVal;
  for i in A1.domain {
    A1(i) = baseVal + iVal;
    A2(i) =           iVal;
    iVal += 1;
  }

  for (b, a1, a2) in zip(B, A1, A2) do b = a1 + a2;    // array follower

  iVal = baseVal;
  var ok = true;
  var spoke = false;
  for i in B.domain {
    ok = ok && B(i) == baseVal + iVal + iVal;
    if verbose && !ok && !spoke {
      writeln('Rank ', r,
              ': first mismatch is at B(', i, '), ', B(i), '!=', iVal);
      spoke = true;
    }
    iVal += 1;
  }

  writeln('Rank ', r, ': ', if ok then 'pass' else 'fail');
}

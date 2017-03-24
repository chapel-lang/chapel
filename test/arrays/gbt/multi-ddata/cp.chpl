use CommDiagnostics;

config const verbose = false;

config const n = defRectArrMultiDDataSizeThreshold + 1;

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

  var A: [D] int;

  var iVal = baseVal;
  for i in A.domain {
    A(i) = baseVal + iVal;
    iVal += 1;
  }

  var B = A;

  iVal = baseVal;
  var ok = true;
  var spoke = false;
  for i in B.domain {
    ok = ok && B(i) == baseVal + iVal;
    if verbose && !ok && !spoke {
      writeln('Rank ', r,
              ': first mismatch is at B(', i, '), ', B(i), '!=', iVal);
      spoke = true;
    }
    iVal += 1;
  }

  writeln('Rank ', r, ': ', if ok then 'pass' else 'fail');
}

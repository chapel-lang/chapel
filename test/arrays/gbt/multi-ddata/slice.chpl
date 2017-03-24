config const verbose = false;

config const n = 2 * defRectArrMultiDDataSizeThreshold;

const baseVal = 12345;

var r = 1..n;
var rs = (r.high/4)..#(r.high/2);
testRank(1, {r}, {rs});

r = 1..ceil(exp(log(n)/2)):int;
rs = (r.high/4)..#(r.high/2);
testRank(2, {r, r}, {rs, rs});

r = 1..ceil(exp(log(n)/3)):int;
rs = (r.high/4)..#(r.high/2);
testRank(2, {r, r, r}, {rs, rs, rs});

proc testRank(rank, D, Ds) {
  if verbose then
    writeln('Rank ', rank, ': ', D, ', slice ', Ds);

  var A1: [D] int;
  var A2: [D] int;
  var B: [D] int;

  var iVal = baseVal;
  for i in A1.domain {
    A1(i) = baseVal + iVal;
    A2(i) =           iVal;
    iVal += 1;
  }

  ref A1s = A1[(...Ds.dims())];
  ref A2s = A2[(...Ds.dims())];
  ref Bs = B[(...Ds.dims())];

  for (b, a1, a2) in zip(Bs, A1s, A2s) do b = a1 + a2;

  var ok = true;
  var spoke = false;
  for i in Bs.domain {
    const expectVal = baseVal + (B.domain.indexOrder(i) + baseVal) * 2;
    ok = ok && Bs(i) == expectVal;
    if verbose && !ok && !spoke {
      writeln('Rank ', rank,
              ': first mismatch is at Bs(', i, '), ', Bs(i), '!=', expectVal);
      spoke = true;
    }
  }

  writeln('Rank ', rank, ': ', if ok then 'pass' else 'fail');
}

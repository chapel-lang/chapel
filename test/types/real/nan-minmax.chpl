
// avoid compile-time optimizations
config const dummy1 = true, dummy2 = true, dummy3 = true, dummy4 = true;
const nan = if dummy1 then NAN else 0.0;
const num = if dummy2 then 1.0 else 1.2;
const plusinf = if dummy3 then INFINITY else 1.3;
const minusinf = if dummy4 then -INFINITY else 1.4;

config const n = 8;
var ARR: [1..n] real = num;

/*
// note that all of these return 'false':
writeln(nan < num);
writeln(nan <= num);
writeln(nan > num);
writeln(nan >= num);
writeln(num < nan);
writeln(num <= nan);
writeln(num > nan);
writeln(num >= nan);
// see also nan-comparisons.chpl
*/

// pair-wise min/max
writeln(min(nan,num), max(nan,num), min(num,nan), max(num,nan));

// reduce ARR with normal contents
assert(min reduce ARR == num);
assert(max reduce ARR == num);
testNans();

// add smallest, largest reals
ARR[1+2] = min(real);
ARR[n-2] = max(real);
assert(min reduce ARR == min(real));
assert(max reduce ARR == max(real));
assert(minloc reduce zip(ARR, ARR.domain) == (min(real), 1+2));
assert(maxloc reduce zip(ARR, ARR.domain) == (max(real), n-2));
testNans();

// also add +- infinity
ARR[1+1] = plusinf;
ARR[n-1] = minusinf;
assert(min reduce ARR == -INFINITY);
assert(max reduce ARR ==  INFINITY);
assert(minloc reduce zip(ARR, ARR.domain) == (-INFINITY, n-1));
assert(maxloc reduce zip(ARR, ARR.domain) == ( INFINITY, 1+1));
testNans();

writeln("done");

// min/max reductions on ARR with 1 or 2 NANs added to it
proc testNans() {
  for i1 in 1..n do
    for i2 in 1..n do
      testNan(ARR, i1, i2);
}
proc testNan(in AA, idx1, idx2) {
  AA[idx1] = nan;
  AA[idx2] = nan;
  assert(isnan(min reduce AA));
  assert(isnan(max reduce AA));

  const minn = minloc reduce zip(AA, AA.domain);
  assert(isnan(minn(0)));
  assert(minn(1) == min(idx1,idx2));

  const maxn = maxloc reduce zip(AA, AA.domain);
  assert(isnan(maxn(0)));
  assert(maxn(1) == min(idx1,idx2));
}

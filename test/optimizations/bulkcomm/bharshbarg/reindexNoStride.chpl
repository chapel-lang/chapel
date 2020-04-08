
use util;

iter halves(dom) {
  var ret : dom.rank*range;
  for i in 0..#2**dom.rank {
    for j in 0..#dom.rank {
      const r = dom.dim(j);
      if i & 1<<j then ret(j) = r[r.low + r.size/2..];
      else ret(j) = r[..r.high - r.size/2];
    }
    yield ret;
  }
}

proc assignHalves(left, right) {
  var ldoms = halves(left.domain);
  var rdoms = halves(right.domain);
  assert(ldoms.size == rdoms.size);
  for (i, j) in zip(0..#ldoms.size, 0..#rdoms.size by -1) {
    stridedAssign(left[(...ldoms[i])], right[(...rdoms[j])]);
  }
}

proc testDim(param rank : int) {
  var ones, zeroes : rank*range;
  for param i in 0..rank-1 {
    ones(i) = 1..8;
    zeroes(i) = 0..7;
  }

  var oneDom = {(...ones)};
  var zeroDom = {(...zeroes)};
  var A, B : [oneDom] int;
  ref oneA = A.reindex(oneDom);
  ref oneB = B.reindex(oneDom);
  ref zeroA = A.reindex(zeroDom);
  ref zeroB = B.reindex(zeroDom);

  stridedAssign(oneA, oneB);
  stridedAssign(zeroA, zeroB);

  assignHalves(oneA, zeroB);
  assignHalves(zeroA, oneB);
}

proc main() {
  for param i in 1..3 {
    testDim(i);
  }
}

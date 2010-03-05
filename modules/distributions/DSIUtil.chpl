// Useful functions for implementing distributions

//
// helper function for blocking index ranges
//
def _computeBlock(numelems, numblocks, blocknum, wayhi,
                  waylo=0:wayhi.type, lo=0:wayhi.type) {
  def intCeilXDivByY(x, y) return ((x + (y-1)) / y);

  if numelems == 0 then
    return (1:lo.type, 0:lo.type);

  const blo =
    if blocknum == 0 then waylo
    else lo + intCeilXDivByY(numelems:lo.type * blocknum:lo.type, numblocks:lo.type);
  const bhi =
    if blocknum == numblocks - 1 then wayhi
    else lo + intCeilXDivByY(numelems:lo.type * (blocknum+1):lo.type, numblocks:lo.type) - 1;

  return (blo, bhi);
}

//
// naive routine for dividing numLocales into rank factors
//
def _factor(param rank: int, value) {
  var factors: rank*int;
  for param i in 1..rank do
    factors(i) = 1;
  if value >= 1 {
    var iv = value;
    var factor = 1;
    while iv > 1 {
      for i in 2..iv {
        if iv % i == 0 {
          var j = 1;
          for i in 2..rank {
            if factors(i) < factors(j) then
              j = i;
          }
          factors(j) *= i;
          iv = iv / i;
          break;
        }
      }
    }
  }
  for i in 1..rank do
    for j in i+1..rank do
      if factors(i) < factors(j) then
        factors(i) <=> factors(j);
  return factors;
}

//
// Returns a new default arithmetic domain of the same rank, index
// type, and shape of 'dom' but for which the indices in each
// dimension start at zero and have unit stride.
//
def computeZeroBasedDomain(dom: domain) {
  def helper(first, rest...) {
    if rest.size > 1 then
      return (0..#first.length, (...helper((...rest))));
    else
      return (0..#first.length, 0..#rest(1).length);
  }
  if dom.rank > 1 then
    return [(...helper((...dom.dims())))];
  else
    return [0..#dom.dim(1).length];
}

//
// setupTargetLocalesArray
//
def setupTargetLocalesArray(targetLocDom, targetLocArr, specifiedLocArr) {
  param rank = targetLocDom.rank;
  if rank != 1 && specifiedLocArr.rank == 1 {
    const factors = _factor(rank, specifiedLocArr.numElements);
    var ranges: rank*range;
    for param i in 1..rank do
      ranges(i) = 0..#factors(i);
    targetLocDom = [(...ranges)];
    targetLocArr = reshape(specifiedLocArr, targetLocDom);
  } else {
    if specifiedLocArr.rank != rank then
      compilerError("specified target array of locales must equal 1 or distribution rank");
    var ranges: rank*range;
    for param i in 1..rank do
      ranges(i) = 0..#specifiedLocArr.domain.dim(i).length;
    targetLocDom = [(...ranges)];
    targetLocArr = specifiedLocArr;
  }
}

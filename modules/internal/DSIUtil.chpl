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


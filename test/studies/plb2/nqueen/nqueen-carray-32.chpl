// this version uses c_array rather than regular arrays

use CTypes;

param NQ_MAX = 31;

proc nq_solve(n: int(32)): int {
  var m: int(32);
  const y0: uint(32) = 1:uint(32)<<n - 1;
  var a: c_array(int(32), NQ_MAX);
  var l,c,r: c_array(uint(32), NQ_MAX);
  for k in 0..<n {
    a[k] = -1;
    // note: l, c, k are already zero-initialized
  }

  var k:int(32) = 0;
  while k >= 0 {
    const y = (l[k] | c[k] | r[k]) & y0; // bit array for possible choices at row k
    if (y ^ y0) >> (a[k] + 1) != 0 { // possible to make a choice
      var i = a[k] + 1;
      while i < n {
        // look for the first choice
        if y & (1 << i) == 0 then break;
        i += 1;
      }
      if k < n - 1 { // store the choice
        const z:uint(32) = 1:uint(32)<<i;
        a[k] = i; k += 1;
        l[k] = (l[k-1]|z)<<1;
        c[k] =  c[k-1]|z;
        r[k] = (r[k-1]|z)>>1;
      } else {
        m += 1; k -= 1; // solution found
      }
    } else {
      a[k] = -1; // no choice; backtrack
      k -= 1;
    }
  }
  return m;
}

config const n:int(32) = 15;
proc main() {
  if n > NQ_MAX || n <= 0 then halt("bad n value");
  writeln(nq_solve(n));
  return 0;
}

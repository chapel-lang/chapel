// This version uses Chapel arrays and 64-bit integers
// Note: 'int' in Chapel is 64-bits. nqueen-32.chpl is
// a port that uses the same data types as the C version.

param NQ_MAX = 31;

proc nq_solve(n: int): int {
  var m: int;
  const y0: int = 1<<n - 1;
  var a: [0..<NQ_MAX] int;
  var l,c,r: [0..<NQ_MAX] int;
  for k in 0..<n {
    a[k] = -1;
    // note: l, c, k are already zero-initialized
  }

  var k:int = 0;
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
        const z:int = 1<<i;
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

config const n:int = 15;
proc main() {
  if n > NQ_MAX || n <= 0 then halt("bad n value");
  writeln(nq_solve(n));
  return 0;
}

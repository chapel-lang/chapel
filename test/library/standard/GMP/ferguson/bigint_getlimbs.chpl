use BigInteger;
use GMP;

// The default is to not output the limbs,
// but to go through the motions of accessing them.
// The reason is that the limb size / configuration
// can vary between platforms.
config const debug = false;

var x:bigint;
x.fac(100);

const n = x.numLimbs;
if debug then writeln("numLimbs is ", n);

var checkn = mpz_size(x.mpz);
assert(n == checkn);

var A:[0..#n] uint;
for i in 0..#n {
  var limb = x.get_limbn(i:uint);
  if debug then writeln("limb ", i, " is ", limb);
  A[i] = limb;
}

// We'll access the limbs in different ways and
// make sure they match.
var B:[0..#n] uint;
var C:[0..#n] uint;

on Locales[numLocales-1] {
  // Check getting size from other locale
  var checkn = x.numLimbs;
  assert(n == checkn);

  // Read each limb from the other locale
  for i in 0..#n {
    var limb = x.get_limbn(i:uint);
    if debug then writeln("limb ", i, " is ", limb);
    B[i] = limb;
  }
}

on Locales[numLocales-1] {
  // Copy the entire number to the current locale.
  var local_x = x;
  
  // Check getting size from copy
  var checkn = local_x.numLimbs;
  assert(n == checkn);

  // Read each limb from the local copy
  for i in 0..#n {
    var limb = local_x.get_limbn(i:uint);
    if debug then writeln("limb ", i, " is ", limb);
    C[i] = limb;
  }
}

// Now make sure that A, B, C match
for i in 0..#n {
  assert(A[i] == B[i]);
  assert(B[i] == C[i]);
}

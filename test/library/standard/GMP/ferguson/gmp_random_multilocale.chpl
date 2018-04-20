use GMP;

config const debug = false;

var A:[1..0] GMPRandom;

// Fill in A with some different RNGs
var def = new GMPRandom();
A.push_back(def);
var mt = new GMPRandom(twister=true);
A.push_back(mt);

/*
 These don't work right now because the store
 a state structure containing pointers / don't
 set the _mp_alloc field in the state mpz.

var a = new bigint(1117);
var lc_2exp = new GMPRandom(a, 117, 9);
A.push_back(lc_2exp);
var lc_2exp_size = new GMPRandom(23);
A.push_back(lc_2exp_size);
*/

// Seed each RNG
for rng in A {
  rng.seed(101);
}

// Copy each RNG locally
var B:[A.domain] GMPRandom;
for (dst,src) in zip(B,A) {
  dst = new GMPRandom(src);
}

// Copy each RNG locally, again
var C:[A.domain] GMPRandom;
for (dst,src) in zip(C,A) {
  dst = new GMPRandom(src);
}

on Locales[numLocales-1] {
  // Copy each RNG from another locale
  var D:[A.domain] GMPRandom;
  for (dst,src) in zip(D,A) {
    dst = new GMPRandom(src);
  }
  // Do it again, for more testing
  var E:[A.domain] GMPRandom;
  for (dst,src) in zip(E,A) {
    dst = new GMPRandom(src);
  }


  // create arrays to store output
  var A_nums:[A.domain] uint;
  var B_nums:[A.domain] uint;
  var C_nums:[A.domain] uint;
  var D_nums:[A.domain] uint;
  var E_nums:[A.domain] uint;

  // Test local number generation
  on Locales[0] {
    for (num, rng) in zip(A_nums, A) {
      num = rng.urandomb(17);
      if debug then writeln("a gen ", num);
    }
  }

  // Test local number generation from copy
  on Locales[0] {
    for (num, rng) in zip(B_nums, B) {
      num = rng.urandomb(17);
      if debug then writeln("b gen ", num);
    }
  }

  // Test remote number generation from local copy
  for (num, rng) in zip(C_nums, C) {
    num = rng.urandomb(17);
    if debug then writeln("c gen ", num);
  }

  // Test local number generation from remote copies
  for (num, rng) in zip(D_nums, D) {
    num = rng.urandomb(17);
    if debug then writeln("d gen ", num);
  }

  // Test remote number generation from remote copies
  on Locales[0] {
    for (num, rng) in zip(E_nums, E) {
      num = rng.urandomb(17);
      if debug then writeln("e gen ", num);
    }
  }

  // Check that the numbers are all the same
  for (a,b,c,d,e) in zip(A_nums, B_nums, C_nums, D_nums, E_nums) {
    assert(a == b);
    assert(b == c);
    assert(c == d);
    assert(d == e);
  }
}

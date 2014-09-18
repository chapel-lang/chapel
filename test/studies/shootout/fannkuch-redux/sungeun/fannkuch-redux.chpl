/***

  fannkuch benchmark from The Computer Language Benchmarks Game
  http://shootout.alioth.debian.org/

  fannkuch.chpl (original) 
  	by: Sung-Eun Choi (sungeun@cray.com)
  fannkuch-redux.chpl (copied and edited from original) 
  	by: Hannah Hemmaplardh (hannah@cray.com) 
	

  Based on the C version by Eckehard Berns which was "Based on code by
  Heiner Marxen and the ATS version by Hongwei Xi".  The spirit of the
  benchmarks seems to have been lost a bit, since the printing of the
  first 30 permutations is done in parallel with the "flipping."  Oh
  well.

  Each program should

    * "Take a permutation of {1,...,n}, for example: {4,2,1,5,3}.
    * Take the first element, here 4, and reverse the order of the
      first 4 elements: {5,1,2,4,3}.
    * Repeat this until the first element is a 1, so flipping won't
      change anything more: {3,4,2,1,5}, {2,4,3,1,5}, {4,2,3,1,5},
      {1,3,2,4,5}.
    * Count the number of flips, here 5.
    * Do this for all n! permutations, and record the maximum number
      of flips needed for any permutation.

  The conjecture is that this maximum count is approximated by n*log(n)
  when n goes to infinity.

  FANNKUCH is an abbreviation for the German word Pfannkuchen, or pancakes,
  in analogy to flipping pancakes."

***/

config param debug: int = 0;
config const N: int = 7;
config const num2print = 30;
var P: domain(1) = {1..N};

proc main() {
  var maxflips: sync int;

  begin maxflips = fannkuch();

  writeln("Pfannkuchen(", N, ") = ", maxflips.readFF());
}

proc fannkuch() {
  return max reduce ([i in P] do_fannkuch(i));
}

proc do_fannkuch(pos: int):int {
  var p, p1, count: [P] int;
  var r: int = N;
  var flips: int = 0;
  var maxflips: int = 0;
  /* var j, k, tmp: int; */
  var k: int;

  for p in gen_perms(N-1, pos) {
    if (p(1) != 1 && p(N) != N) {
      flips = 0;
      k = p(1);
      if (debug > 0) {
	write("Starting: ");
	for i in P do
	  write(p(i));
	writeln();
      }
      do {
	if (debug > 0) {
	  write("[", pos, "][", k, "]");
	}
	// "physically" flip only elements 2..k-1
	// j = k-1;
	for (i,j) in zip(2..k-1, ..k-1 by -1) do { // use range instead of domain
	  if i > j then break;
	  if (debug > 1) {
	    write((i,j));
	  }
	  p(i) <=> p(j);
	}
	// set p(k) = p(1)
	k <=> p(k);
	flips += 1;

	if (debug > 0) {
	  write(k);
	  for i in 2..N do
	    write(p(i));
	  writeln(": ", flips);
	}
      } while (k != 1);
      if maxflips < flips then
	maxflips = flips;
    }
  }

  return maxflips;
}


// iterator for all possible permutations
iter gen_perms(limit: int, swap:int=-1): [P] int {
  var p, result: [P] int;
  var count: [P] int;
  var r: int = N;

  // Could use:
  //   p = P;
  for i in P do
    p(i) = i;
  if swap!=-1 then p(swap)<=>p(N);

  label outer while (1) {
    result = p;
    yield result;

    while (r > 1) {
      count(r) = r;
      r -= 1;
    }

    label inner while (1) {
      if r >= limit then break outer;

      var p0: int = p(1);
      for i in 1..r do
	p(i) = p(i + 1);
      p(r+1) = p0;

      count(r+1) -= 1;
      if count(r+1) > 0 then break inner;

      r += 1;
    }
  }
}


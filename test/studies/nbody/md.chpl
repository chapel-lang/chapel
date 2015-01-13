/*
This test is inspired by an example from Michael Dietrich.
It bears similarity to n-body.

Since computeOne() is very simple, the test currently emphasizes
communication costs and array/iteration overhead.

A potential improvement is to change data movements so that each locale
receives only from its "left" neighbor and sends only to its "right" neighbor.
*/

use Time;
use BlockDist;

config const
  test1 = true,
  test2 = true,
  timing = false;

config const n = 1000;
const D_dr = {1..n};

const D_distrib = D_dr dmapped Block(D_dr);
var A, B: [D_distrib] real;

proc computeOne(b_i, b_j) {
  return b_i + b_j;
}

// a simple version of the algorithm
proc try1() {

  var tm: Timer;
  tm.start();

  // "forall": parallel, distributed over D_distrib
  // compute all A[i] in parallel
  forall i in D_distrib {

    // "for": sequential, local; over a privitized copy of D_distrib
    // accumulate into A[i] sequentially
    for j in D_distrib {

      // some computation
      A[i] += computeOne(B[i], B[j]);

    }
  }

  tm.stop();
  if timing then writeln("no caching: ", tm.elapsed(), " seconds");
}

// overlap computation and communication
proc try2() {

  var tm: Timer;
  tm.start();

  // parallel over locales
  coforall loc_i in 0..#numLocales do on Locales[loc_i] {

      // local indices and slices of A, B
      const local_dom => B.localSubdomain();
      var local_A => A.localSlice(local_dom);
      var local_B => B.localSlice(local_dom);

      // local cache of a chunk of B used for computation
      // starting with the local portion
      var curr_dom = local_dom;
      var curr_array: [curr_dom] real;

      // local cache of the prefetched chunk of B
      var next_dom: curr_dom.type;
      var next_array: [next_dom] real;

      // process 'numLocales' chunks of B
      for loc_j in 0..#numLocales {

        const next_loc = (loc_i + loc_j + 1) % numLocales;
        const more_chunks = loc_j < numLocales-1;

        cobegin {

          // Task 1: prefetch into 'next_array'
          if more_chunks then on Locales[next_loc] {
            const local_subdom => B.localSubdomain();
            next_dom = local_subdom;
            next_array = B.localSlice(local_subdom);
          }

          // Task 2: compute with prefetched values in 'curr_array'
          // parallel within the locale
          forall (a_i, b_i) in zip(local_A, local_B) {
            for b_j in curr_array {
              a_i += computeOne(b_i, b_j);
            }
          }

        } // cobegin


        // move prefetched values from 'next_array' into 'curr_array'
        if more_chunks {
          curr_dom = next_dom;
          curr_array = next_array;
        }

      } // for loc_j

  } // coforall loc_i

  tm.stop();
  if timing then writeln("with caching: ", tm.elapsed(), " seconds");
}

proc main() {
  writeln("start");
  if test1 then try1();
  if test2 then try2();
  writeln("done");
}

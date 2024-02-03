
/*****
      ex_quicksort.chpl -
      Implementation of Quicksort following "Introduction to Algorithms",
      Corman et al. 2nd Ed. Chapter 7.1.  Runs in parallel.  Uses an insertion
      sort for small arrays, Chapter 2.1.

      Call:
        ex_quicksort

      c 2015-2018 Primordial Machine Vision Systems
*****/

use Random;

/* The Chapel runtime blows through the stack pretty quickly running this.
   You'll start seeing stack allocation failures if nelt is increased or
   len_insert decreases.  The program will still run and pass verification,
   but you'll see a stream of error messages on the console.  Eventually
   the program does completely crash.
   Increasing len_insert stops the recursion earlier, which allows a higher
   nelt.
   Compiling with --fast removes the warning messages.  You can get an large
   increase in nelt before the program starts crashing.
*/
config param nelt = 1000;               /* size of test case */
config param len_insert = 16;           /* size array to use insertion sort */
config param use_insert = false;        /* true to insertionsort small arrays */

/***
    quicksort:  Sort an array in-place, in parallel.
    args:       data - array to sort
                rng - region of array to process with this pass
    modifies:  data
***/
proc quicksort(ref data : [] real) {
  var q : int;                          /* index of pivot */

  if (data.domain.first < data.domain.last) {
    if (use_insert &&
        ((data.domain.last - data.domain.first + 1) <= len_insert)) then
      insertsort(data);
    else {
      q = partition(data);
      cobegin {
        quicksort(data[..q-1]);
        quicksort(data[q+1..]);
      }
    }
  }
}

/***
    partition:  Re-arrange an array so that all elements before the value at
                the end of the array are to the left, all larger are to the
                right.  Returns the index where the value at the end ends up.
    args:       data - array to sort
    returns:   index of element that's now in place
    modifies:  data
***/
proc partition(ref data : [] real) : int {

  const pivot = data(data.domain.last);
  var i = data.domain.first - 1;
  for j in data.domain.first..data.domain.last-1 do
    if (data(j) <= pivot) {
      i += 1;
      data(i) <=> data(j);
    }

  i += 1;
  data(i) <=> data(data.domain.last);

  return i;
}

/***
    insertsort:  Perform an in-place insertion sort on an array.  Intro to
                 Algorithms Chapter 2.1.
    args:        data - array to sort
    modifies:  data
***/
proc insertsort(data : [] real) {

  for j in data.domain.first+1..data.domain.last {
    const key = data(j);
    var i = j - 1;
    while ((i >= data.domain.first) && (data(i) > key)) {
      data(i+1) = data(i);
      i -= 1;
    }
    data(i+1) = key;
  }
}

proc main() {
  const rand = createRandomStream(real, 1);    /* RNG to create data */
  var data : [1..nelt] real;            /* array to sort */
  var passed = true;                    /* verification flag */

  for i in 1..nelt do data(i) = rand.getNext();

  quicksort(data);

  /* Verify. */
  for i in 2..nelt do
    if (data(i) < data(i-1)) {
      writeln("sort failed at ind ", i, "  data ", data(i), " > ", data(i-1));
      passed = false;
    }

  if (!passed) then writeln("--> sort FAILED verification");
  else writeln("--> sort passed verification");
}

/*
   This primer demonstrates how to work with C functions
   (called "extern functions" by Chapel). More information
   is available in doc/rst/technotes/extern.rst.
 */


// Use the Time module to time some C vs Chapel tests
use Time;

// The quiet configuration constant is set to false when testing (via
// start_test) so that this test is deterministic.
config const quiet = false;

config const n = 100000;

// This block contains C code that will be included in the Chapel
// program. The symbols created in this block, including those
// defined in header files that are #include'd, will be available
// for the Chapel code below to use.
extern {

  #include <stdint.h>

  // make puts available for the example below
  #include <stdio.h>

  // make gethostname available for example below 
  #include <unistd.h>

   // this function can be inlined into the Chapel code that uses
  // it since it's declared inline here (depending on whether or not
  // the back-end compiler follows the inline hint.
  static inline int64_t mysum(int64_t *array, int length) {
    int64_t accum = 0;
    for( int i = 0; i < length; i++ ) {
      accum += array[i];
    }
    return accum;
  }
}

// create an array A containing 1..n
var A = [i in 1..n] i;

var sum_chapel: int;
var sum_c: int;
var t: stopwatch;

// compare timings between serial C and Chapel array sums:
t.start();
sum_chapel = + reduce A;
t.stop();
writeln("Sum from Chapel was ", sum_chapel);
if !quiet then writeln("Chapel sum took ", t.elapsed(), " seconds");

t.clear();
t.start();
sum_c = mysum(c_ptrTo(A), A.size:c_int);
t.stop();
writeln("Sum from serial C was ", sum_c);
if !quiet then writeln("serial C sum took ", t.elapsed(), " seconds");

assert(sum_chapel == sum_c);


// Now try gethostname and puts
var hostname_ptr: c_ptr(c_char);
var hostname_len = 100;
var result:c_int;

hostname_ptr = c_calloc(c_char, hostname_len);

result = gethostname(hostname_ptr:c_ptr(c_char), hostname_len:c_size_t);
if !quiet {
  if result == 0 {
    writeln("gethostname returned:");
    // write out the hostname too
    puts(hostname_ptr:c_string);
    writeln("\n");
  } else {
    writeln("gethostname returned an error");
  }
}

// Demonstrate c_calloc and c_free
c_free(hostname_ptr);




// The following two routines are currently required in this module
// but should be in the standard modules.

// allow casts from c_ptr(c_char) to c_string
pragma "no doc"
inline operator :(x, type t) where isSubtype(t,c_string) && isSubtype(x.type,c_ptr(c_char)) {
    return __primitive("cast", t, x);
}

// allow casts from c_string to c_ptr(c_char)
pragma "no doc"
inline operator :(x, type t) where isSubtype(t,c_ptr(c_char)) && isSubtype(x.type,c_string) {
    return __primitive("cast", t, x);
}




use Sys;
//
// This test checks the error message printed by the comm=ugni SIGBUS
// handler when we run out of memory while trying to allocate an array
// on hugepages separate from the heap, with dynamic registration.  The
// default config const value here is chosen so as to overflow even on
// a 256g compute node.
//
var evHNR: c_string;
const evHNRSet = sys_getenv(c'HUGETLB_NO_RESERVE', evHNR) != 0;
if !evHNRSet || (evHNRSet && (evHNR:string).toLower() != 'yes') then
  writeln('Without HUGETLB_NO_RESERVE=yes, test may not work as expected.');

config const arraySize = 256 * 2**30;

var x: [1..arraySize] int(8);

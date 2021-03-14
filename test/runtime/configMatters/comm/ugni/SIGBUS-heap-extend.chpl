use Sys;
//
// This test checks the error message printed by the comm=ugni SIGBUS
// handler when we run out of memory trying to extend the heap, with
// a dynamically extendable registered heap on hugepages.  It creates
// more arrays, each small enough that it is heap-allocated, than the
// compute node has memory to hold.  The default config const values
// here are chosen such that we overflow even on a 256g compute node.
//
// Note that the program must be run with a hugepage size large enough
// that fewer than 1024 hugepages will span memory, because otherwise
// what the program will run out of first is comm=ugni memory region
// table entries and you'll get a different error message.  So for
// example, on a 128g XC node use at least 256m hugepages.
//
var evHNR: c_string;
const evHNRSet = sys_getenv(c'HUGETLB_NO_RESERVE', evHNR) != 0;
if !evHNRSet || (evHNRSet && (evHNR:string).toLower() != 'yes') then
  writeln('Without HUGETLB_NO_RESERVE=yes, test may not work as expected.');

config const arraySize = 2**20;
config const arraysPerTask = 256 * 2**30 / arraySize / here.maxTaskPar;

coforall 1..here.maxTaskPar {
  var x: [1..arraysPerTask] [1..arraySize] int(8);
}

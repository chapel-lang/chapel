// this test was motivated by a bug report from Arkouda:
//
//   https://github.com/mhmerrill/arkouda/issues/236
//
// before the PR that this got in, this code used to get segfaults with ugni,
// due to a bug


use BlockDist;

// without this we are allocating way too much memory for VMs
config var n = if CHPL_COMM=="ugni" then 600_000_001 else 60_000_001;

var d1 = {0..#n} dmapped Block({0..#n});
var a1: [d1] bool;

var d2 = {0..#n} dmapped Block({0..#n});
var a2: [d2] bool;

var d3 = {0..#(2*n)} dmapped Block({0..#(2*n)});
var a3: [d3] bool;

a3[{0..#n}] = a1;
a3[{n..#n}] = a2;

writeln(a3[3]);

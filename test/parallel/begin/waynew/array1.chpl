// Test a begin block with a reference to a global array and whether
// arrays are thread safe.  A sort of weak test on the latter.

// One computation sets the first half of an array.  A second
// computation sets the second half and then signals the first
// when it can print out the results via a single bool.

param N = 20;
var a: [1..N] int;
var done: single bool;


for i in 1..N/2 do
  a[i] = i;

begin {
  for j in (N/2+1)..N do
    a[j] = N+j-(N/2);
  done.writeEF(true);
}

var bogus = done;
writeln( a);

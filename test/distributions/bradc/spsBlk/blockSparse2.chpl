//
// We need to use the block distribution module.
//
use BlockDist;

//
// The per-dimension size of the array.  Override using --n <val> on
// the execution command line.
//
config const n = 8;

//
// The block-distributed dense domain that will serve as the parent
// domain for the sparse one.
//
const Elems = {0..#n, 0..#n} dmapped Block({0..#n, 0..#n});

//
// The sparse subdomain.  In the current code framework, the parent
// domain gets to pick the domain map for the sparse and (I believe)
// can't be overridden (though ultimately we want/need this ability).
// Block-distributed domains currently pick Block-COO as their sparse
// representation.
//
var MatElems: sparse subdomain(Elems);

//
// Populate the sparse domain.  Note that for best performance (a)
// adding indices in pre-sorted order works best and (b) declaring
// your sparse array after your sparse domain has been finalized is
// ideal.
//
for i in 0..#n {
  MatElems += (i,i);
  MatElems += (i,n-i-1);
}

//
// Printing multidimensional sparse domains doesn't work yet.
//
//writeln("MatElems = ", MatElems);

//
// Declare a dense array over the parent domain.
//
var Dns: [Elems] int;

//
// For all indices in the sparse domain, set the dense value as a
// function of the owning locale's ID.
//
forall ij in MatElems do
  Dns[ij] = here.id + 1;

//
// Print the dense array.
//
writeln("Dns is:\n", Dns);

//
// Declare a sparse array
//
var Sps: [MatElems] int;

//
// Fill it similarly to the dense above.
//
forall ij in MatElems do
  Sps[ij] = here.id + 1;

//
// Future work:  How would we want this to print out?
//
// writeln("Sps is:\n", Sps);
//

//
// Manually print out the sparse array as a dense array for now.
//
writeln("Sps is:");
for i in Elems.dim(0) do
  for j in Elems.dim(1) {
    write(Sps[i,j]);
    if (j == Elems.dim(1).high) then
      writeln();
    else
      write(" ");
  }



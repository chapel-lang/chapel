//
// We need to use the block distribution module.
//
use BlockDist;

//
// The size of the array.  Override using --n <val> on the execution
// command line.
//
config const n = 8;

//
// The block-distributed dense domain that will serve as the parent
// domain for the sparse one.
//
const Elems = {0..#n} dmapped Block({0..#n});

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

var totalElts = 0;
for i in 0..#n by 2 {
  MatElems += i;
  totalElts += 1;
}

// Check that the sparse domain has the correct size.
assert(MatElems.size == totalElts);

//
// Print the sparse index set.
//
writeln("MatElems = ", MatElems);

//
// Declare a dense array over the parent domain.
//
var Dns: [Elems] int;

//
// For all indices in the sparse domain, set the dense array's value
// as a function of the owning locale's ID.
//
forall i in MatElems do
  Dns[i] = here.id + 1;

//
// Print the dense array.
//
writeln("Dns is: ", Dns);

//
// Declare a sparse array
//
var Sps: [MatElems] int;

//
// Fill it similarly to the dense above.
//
forall i in MatElems do
  Sps[i] = here.id + 1;

//
// Print the sparse array.
//
writeln("Sps is: ", Sps);

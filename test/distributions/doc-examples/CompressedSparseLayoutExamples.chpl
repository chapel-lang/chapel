//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

const m = 32;
const n = 64;

iter indices() {
  for (i, j) in zip(1..4, 8..11) do yield (i, j);
}

/* START_EXAMPLE */
use CompressedSparseLayout;

const D = {1..m, 1..n};   // a dense domain defining a bounding box
const CSRDomain: sparse subdomain(D) dmapped new csrLayout() = indices();

var CSRArray: [CSRDomain] real;
/* STOP_EXAMPLE */

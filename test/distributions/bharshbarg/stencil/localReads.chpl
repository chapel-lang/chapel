
use StencilDist;

config const n = 100;

const halo = (2,2);

proc test(A: []) {
  //
  // Confirm that indexing into the cached region does not result in
  // communication.
  //
  for loc in Locales do on loc {
    const locDom = A.domain.localSubdomain();
    if locDom.size > 0 {
      const max = locDom.expand(halo);
      forall idx in max do local {
        if A[idx] == -1 then
          halt("Incorrect value in cache");
      }
    }
  }
}

proc main() {
  const Dom = {1..n, 1..n};
  var Space = Dom dmapped Stencil(Dom, fluff=halo, periodic=true);

  var A : [Space] int;
  [(i,j) in Space] A[i,j] = i*n+j;
  A.updateFluff();

  test(A);

  // test reads outside of the slicing domain which only works if that
  // slicing domain is a Stencil domain.  So the following slices
  // create one by slicing the Stencil domain with the restricted
  // index set.

  const sub = Space[Space.localSubdomain()];
  test(A[sub]);

  const mid = Space[{n/4..#n/2, n/4..#n/2}];
  test(A[mid]);

  writeln("Success");

  // The following demonstrate the OOB errors that occur if simply
  // slicing with a default rectangular (DR) domain and then trying to
  // read the slice out of bounds.

  const subDR = Space.localSubdomain();
  test(A[subDR]);

  const midDR = {n/4..#n/2, n/4..#n/2};
  test(A[midDR]);
}


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

  const sub = Space.localSubdomain();
  test(A[sub]);

  const mid = {n/4..#n/2, n/4..#n/2};
  test(A[mid]);

  writeln("Success");
}

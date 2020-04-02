proc testSparse(type t) {
  /* non-nilable shared T */

  var D = {1..100};
  var sparseD: sparse subdomain(D);

  sparseD += 1;

  var A: [sparseD] t;
  A[1] = if isTuple(t) then (new t[1](1), new t[2](2))
          else new t(1);

  assert(A.size == 1);
}

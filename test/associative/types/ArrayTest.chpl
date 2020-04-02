proc testArray(type t) {
  /* non-nilable shared T */

  var D: domain(int);

  D += 1;

  var A: [D] t;
  A[1] = if isTuple(t) then (new t[0](1), new t[1](2))
          else new t(1);


  assert(A.size == 1);
}

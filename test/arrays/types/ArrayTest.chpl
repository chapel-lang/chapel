proc testArray(type t) {
  /* non-nilable shared T */

  var a = if isTuple(t) then [(new t[1](1), new t[2](2))]
          else [new t(1)];


  assert(a.size == 1);
}

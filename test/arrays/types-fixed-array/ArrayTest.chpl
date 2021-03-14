proc testArray(type t) {
  /* non-nilable shared T */

  var a = if isTuple(t) then [(new t[0](1), new t[1](2))]
          else [new t(1)];


  assert(a.size == 1);

  if isUnmanagedClass(t) {
    delete a;
  }
}

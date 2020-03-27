proc test(type t) {
  /* non-nilable shared T */

  var a = if isTuple(t) then ((new t[1](1), new t[2](2)), (new t[1](3), new t[2](4)))
          else (new t(1), new t(2));


  assert(a.size == 2);
}

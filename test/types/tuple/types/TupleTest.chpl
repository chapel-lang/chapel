proc test(type t) {
  /* non-nilable shared T */

  var a = if isTuple(t) then ((new t[0](1), new t[1](2)), (new t[0](3), new t[1](4)))
          else (new t(0), new t(1));


  assert(a.size == 2);
}

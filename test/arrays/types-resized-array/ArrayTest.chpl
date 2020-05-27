proc testArray(type t) {

  var D = {0..0};
  var a: [D] t = if isTuple(t) then [(new t[0](1), new t[1](2))]
                  else [new t(1)];

  assert(a.size == 1);

  D = {0..1};

  assert(a.size == 2);

  if isUnmanagedClass(t) {
    delete a;
  }
}


class ArrayViewSliceArr: BaseArr {
  type eltType;
  const dom;
  const arr;

  //
  // TODO: Could this be replaced with more type-based introspection?
  // I shied away from it since this is a generic class, but there
  // must be (or should be) some way to do it without relying on
  // methods like this...
  //
  proc isSliceArrayView() param {
    return true;
  }

  inline proc dsiGetBaseDom() {
    return dom;
  }

  //
  // standard iterators
  //
  inline iter these() ref {
    for i in dom do
      yield arr.dsiAccess(i);
  }

  inline iter these(param tag: iterKind) ref where tag == iterKind.standalone {
    for i in dom.these(tag) do
      yield arr.dsiAccess(i);
  }

  inline iter these(param tag: iterKind) where tag == iterKind.leader {
    //    writeln("In sliceview leader");
    for followThis in dom.these(tag) do {
      //      writeln("yielding ", followThis);
      yield followThis;
    }
  }

  inline iter these(param tag: iterKind, followThis) ref
    where tag == iterKind.follower {
    for i in dom.these(tag, followThis) do
      yield arr.dsiAccess[i];
  }

  //
  // standard I/O stuff
  //
  proc dsiSerialWrite(f) {
    chpl_serialReadWriteRectangular(f, arr, dom);
  }

  //
  // standard accessors
  //
  inline proc dsiAccess(i:integral) ref {
    return dsiAccess((i,));
  }

  inline proc dsiAccess(i) ref {
    //    writeln("Slice got: ", i);
    if boundsChecking then
      if !dom.dsiMember(i) then
        halt("array index out of bounds: ", i);

    return arr.dsiAccess(i);
  }
}


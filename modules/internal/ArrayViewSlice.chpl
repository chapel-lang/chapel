class ArrayViewSliceArr: BaseArr {
  type eltType;  // see note on commented-out proc eltType below...

  // TODO: Can we privatize upon creation of the array-view slice and cache
  // the results?
  const _DomPid;
  const dom; // Seems like the compiler requires a field called 'dom'...

  const _ArrPid;
  const _ArrInstance;

  inline proc privDom {
    if _isPrivatized(dom) {
      return chpl_getPrivatizedCopy(dom.type, _DomPid);
    } else {
      return dom;
    }
  }

  inline proc arr {
    if _isPrivatized(_ArrInstance) {
      return chpl_getPrivatizedCopy(_ArrInstance.type, _ArrPid);
    } else {
      return _ArrInstance;
    }
  }

  proc idxType type return dom.idxType;
  proc rank param return arr.rank;

  // This seems like it ought to work, but it causes an error in the
  // compiler for non-devel mode...  presumably due to a direct
  // query of eltType...
  //  proc eltType type return arr.eltType;

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
    for i in privDom do
      yield arr.dsiAccess(i);
  }

  inline iter these(param tag: iterKind) ref where tag == iterKind.standalone {
    for i in privDom.these(tag) do
      yield arr.dsiAccess(i);
  }

  inline iter these(param tag: iterKind) where tag == iterKind.leader {
    //    writeln("In sliceview leader");
    for followThis in privDom.these(tag) do {
      //      writeln("yielding ", followThis);
      yield followThis;
    }
  }

  inline iter these(param tag: iterKind, followThis) ref
    where tag == iterKind.follower {
    for i in privDom.these(tag, followThis) do
      yield arr.dsiAccess[i];
  }

  //
  // standard I/O stuff
  //
  proc dsiSerialWrite(f) {
    chpl_serialReadWriteRectangular(f, arr, privDom);
  }

  inline proc checkBounds(i) {
    if boundsChecking then
      if !privDom.dsiMember(i) then
        halt("array index out of bounds: ", i);
  }

  //
  // standard accessors
  //
  inline proc dsiAccess(i: idxType ...rank) ref {
    return dsiAccess(i);
  }

  inline proc dsiAccess(i: idxType ...rank)
  where !shouldReturnRvalueByConstRef(eltType) {
    return dsiAccess(i);
  }

  inline proc dsiAccess(i: idxType ...rank) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    return dsiAccess(i);
  }

  inline proc dsiAccess(i) ref {
    checkBounds(i);
    return arr.dsiAccess(i);
  }

  inline proc dsiAccess(i)
  where !shouldReturnRvalueByConstRef(eltType) {
    checkBounds(i);
    return arr.dsiAccess(i);
  }

  inline proc dsiAccess(i) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    checkBounds(i);
    return arr.dsiAccess(i);
  }

  //
  // bulk transfer routines -- forward to array
  //
  proc doiBulkTransferToDR(B) {
    arr.doiBulkTransferToDR(B);
  }

  /*  I don't think these should be needed...
  proc doiBulkTransferStride(B) {
    arr.doiBulkTransferStride(B);
  }

  proc dataChunk(x) ref {
    return arr.dataChunk(x);
  }
*/

  proc dsiReindex(newdom) {
    compilerError("reindexing not supported on array views yet");
  }

  proc dsiRankChange() {
    compilerError("rank change not supported on array views yet");
  }

  proc dsiNoFluffView() {
    if canResolveMethod(arr, "dsiNoFluffView") {
      return arr.dsiNoFluffView();
    } else {
      compilerError("noFluffView is not supported on this array type.");
    }
  }

  //
  // Local subdomain interface
  //
  proc dsiHasSingleLocalSubdomain() param
    return privDom.dsiHasSingleLocalSubdomain();

  //
  // TODO: Is this correct in distributed memory?
  //
  proc dsiLocalSubdomain() {
    return privDom.dsiLocalSubdomain();
  }

  proc dsiSupportsPrivatization() param return true;

  proc dsiGetPrivatizeData() {
    return (_DomPid, dom, _ArrPid, _ArrInstance);
  }

  proc dsiPrivatize(privatizeData) {
    return new ArrayViewSliceArr(eltType=this.eltType, _DomPid=privatizeData(1),
                                 dom=privatizeData(2),
                                 _ArrPid=privatizeData(3),
                                 _ArrInstance=privatizeData(4));
  }
}


class ReferenceCount {
  var count: atomic int;

  // count should be initialized to 1 in default initializer.
  // Pretty much any strategy can do that.
  // There is the wrinkle that initializing an atomic
  // currently happens with a.write(1). We could presumably
  // support = for initialing atomics, however.
  proc ReferenceCount() {
    //count = 1;      // this should work
    //super.init();
    count.write(1); // how you'd write it now
  }

  proc retain() {
    count.add(1);
  }
  proc release() {
    var oldValue = count.fetchSub(1);
    return oldValue - 1;
  }
}


record RefCounted {
  var p;                 // contained pointer (class type)
  var pn:ReferenceCount; // reference counter

  // forward method invocations to p.
  delegate p;

  proc RefCounted(p) {

    // Boost version default-initializes px and pn
    // and then swaps in different values.

    if !isClass(p.type) then
      compilerError("RefCounted only works with classes");

    this.p = p;
    this.pn = new ReferenceCount();

    //super.init();

    // Boost includes a mechanism for classes inheriting from
    // enable_shared_from_this to record a weak pointer back to the
    // shared pointer. That would need to be handled in a Phase 2
    // since it would refer to `this` as a whole here.
  }

  proc ~RefCounted() {
    release();
  }


  proc retain() {
    pn.retain();
  }

  proc release() {
    var count = pn.release();
    if count == 0 {
      delete p;
      delete pn;
    }
  }

  // = should call retain-release
  // copy-init should call retain
}

pragma "init copy fn"
pragma "no doc"
proc chpl__initCopy(src: RefCounted) {
  // This pragma may be unnecessary.
  pragma "no auto destroy"
  var ret:src.type;
  ret.p = src.p;
  ret.pn = src.pn;
  ret.retain();
  return ret;
}

pragma "donor fn"
pragma "auto copy fn"
pragma "no doc"
proc chpl__autoCopy(src: RefCounted) {
  // This pragma may be unnecessary.
  pragma "no auto destroy"
  var ret:src.type;
  ret.p = src.p;
  ret.pn = src.pn;
  ret.retain();
  return ret;
}

proc =(ref lhs:RefCounted, rhs: RefCounted) {
  // retain-release
  rhs.retain();
  lhs.release();
  lhs.p = rhs.p;
  lhs.pn = rhs.pn;
}


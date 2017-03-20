
pragma "no copy"
record Owned {
  var p;                 // contained pointer (class type)

  proc Owned(p) {
    if !isClass(p.type) then
      compilerError("Owned only works with classes");

    this.p = p;
    //super.init();
  }

  // No copy-init is defined
  // no copy may be made

  proc ~Owned() {
    if p then
      delete p;
  }

  proc ref reset(newPtr:p.type) {
    var oldPtr = p;
    p = newPtr;
    if oldPtr then
      delete oldPtr;
  }

  proc ref release() {
    var oldPtr = p;
    p = nil;
    return oldPtr;
  }

  proc /*const*/ get() {
    return p;
  }
}

proc =(ref lhs:Owned, ref rhs: Owned) {
  compilerError("Owned may not be assigned. Use lhs.reset(rhs.release()).");
  //lhs.reset(rhs.release());
}

// workaround for problems with generic initializers
pragma "init copy fn"
pragma "no doc"
pragma "erroneous initcopy"
proc chpl__initCopy(src: Owned) {
  return src;
}



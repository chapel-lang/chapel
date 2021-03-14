class C { var x: int; }

proc checkSameType(type a, type b) {
  if a != b then
    compilerError("failure: " + a:string + " != " + b:string);
}

proc checkSameManagement(type a, type b) {
  if isOwnedClassType(a) == isOwnedClassType(b) &&
     isSharedClassType(a) == isSharedClassType(b) &&
     isUnmanagedClassType(a) == isUnmanagedClassType(b) &&
     isBorrowedClassType(a) == isBorrowedClassType(b) {
    // OK
  } else {
    compilerError("failure: " + a:string + " diff mgmt " + b:string);
  }
}

proc checkSameNilability(type a, type b) {
  if isNilableClassType(a) == isNilableClassType(b) &&
     isNonNilableClassType(a) == isNonNilableClassType(b) {
    // OK
  } else {
    compilerError("failure: " + a:string + " diff nil " + b:string);
  }
}

// CC is the undecorated, non-nilable version
proc test1(type t, type CC) {
  if isNilableClassType(t) {
    checkSameType(t:owned, owned CC?);
    checkSameType(t:shared, shared CC?);
    checkSameType(t:unmanaged, unmanaged CC?);
    checkSameType(t:borrowed, borrowed CC?);
    checkSameType(_to_unmanaged(t), unmanaged CC?);
    checkSameType(_to_borrowed(t), borrowed CC?);
  } else {
    checkSameType(t:owned, owned CC);
    checkSameType(t:shared, shared CC);
    checkSameType(t:unmanaged, unmanaged CC);
    checkSameType(t:borrowed, borrowed CC);
    checkSameType(_to_unmanaged(t), unmanaged CC);
    checkSameType(_to_borrowed(t), borrowed CC);
  }
  checkSameManagement(t:owned, owned CC);
  checkSameNilability(t:owned, t);
  checkSameManagement(t:shared, shared CC);
  checkSameNilability(t:shared, t);
  checkSameManagement(t:unmanaged, unmanaged CC);
  checkSameNilability(t:unmanaged, t);
  checkSameManagement(t:borrowed, borrowed CC);
  checkSameNilability(t:borrowed, t);

  checkSameType(t:owned class, owned CC);
  checkSameType(t:shared class, shared CC);
  checkSameType(t:unmanaged class, unmanaged CC);
  checkSameType(t:borrowed class, borrowed CC);
  
  checkSameType(t:owned class?, owned CC?);
  checkSameType(t:shared class?, shared CC?);
  checkSameType(t:unmanaged class?, unmanaged CC?);
  checkSameType(t:borrowed class?, borrowed CC?);

  if isNilableClassType(t) {
    checkSameType(t:class?, t);
  } else {
    checkSameType(t:class, t);
  }
  checkSameManagement(t:class, t);
  checkSameManagement(t:class?, t);
}
proc test2(type t) {
  if isNilableClassType(t) {
    checkSameType(t:owned, owned C?);
    checkSameType(t:shared, shared C?);
    checkSameType(t:unmanaged, unmanaged C?);
    checkSameType(t:borrowed, borrowed C?);
    checkSameType(_to_unmanaged(t), unmanaged C?);
    checkSameType(_to_borrowed(t), borrowed C?);
  } else {
    checkSameType(t:owned, owned C);
    checkSameType(t:shared, shared C);
    checkSameType(t:unmanaged, unmanaged C);
    checkSameType(t:borrowed, borrowed C);
    checkSameType(_to_unmanaged(t), unmanaged C);
    checkSameType(_to_borrowed(t), borrowed C);
  }

  checkSameType(t:owned class, owned C);
  checkSameType(t:shared class, shared C);
  checkSameType(t:unmanaged class, unmanaged C);
  checkSameType(t:borrowed class, borrowed C);
  
  checkSameType(t:owned class?, owned C?);
  checkSameType(t:shared class?, shared C?);
  checkSameType(t:unmanaged class?, unmanaged C?);
  checkSameType(t:borrowed class?, borrowed C?);

  if isNilableClassType(t) {
    checkSameType(t:class?, t);
  } else {
    checkSameType(t:class, t);
  }
}

proc test(type t, type CC) {
  test1(t, CC);
  test2(t);
}

proc main() {
  test(C, C);
  test(owned C, C);
  test(shared C, C);
  test(unmanaged C, C);
  test(borrowed C, C);
  
  test(owned C?, C);
  test(shared C?, C);
  test(unmanaged C?, C);
  test(borrowed C?, C);

  checkSameType(_to_nonnil(owned C), owned C);
  checkSameType(_to_nonnil(owned C?), owned C);
  checkSameType(_to_nilable(owned C), owned C?);
  checkSameType(_to_nilable(owned C?), owned C?);
  checkSameType(_to_nonnil(shared C), shared C);
  checkSameType(_to_nonnil(shared C?), shared C);
  checkSameType(_to_nilable(shared C), shared C?);
  checkSameType(_to_nilable(shared C?), shared C?);
  checkSameType(_to_nonnil(unmanaged C), unmanaged C);
  checkSameType(_to_nonnil(unmanaged C?), unmanaged C);
  checkSameType(_to_nilable(unmanaged C), unmanaged C?);
  checkSameType(_to_nilable(unmanaged C?), unmanaged C?);
  checkSameType(_to_nonnil(borrowed C), borrowed C);
  checkSameType(_to_nonnil(borrowed C?), borrowed C);
  checkSameType(_to_nilable(borrowed C), borrowed C?);
  checkSameType(_to_nilable(borrowed C?), borrowed C?);
}

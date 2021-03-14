class C { var x: int; }

// cast various memory flavors to C / C?

proc testTypes() {
  writeln(C  :C? :string);
  writeln(C? :C  :string);
  writeln(C? :C? :string);
  writeln(C  :C  :string);

  writeln(owned C  :C? :string);
  writeln(owned C? :C  :string);
  writeln(owned C? :C? :string);
  writeln(owned C  :C  :string);

  writeln(shared C  :C? :string);
  writeln(shared C? :C  :string);
  writeln(shared C? :C? :string);
  writeln(shared C  :C  :string);

  writeln(borrowed C  :C? :string);
  writeln(borrowed C? :C  :string);
  writeln(borrowed C? :C? :string);
  writeln(borrowed C  :C  :string);

  writeln(unmanaged C  :C? :string);
  writeln(unmanaged C? :C  :string);
  writeln(unmanaged C? :C? :string);
  writeln(unmanaged C  :C  :string);
}
testTypes();

proc borrowedToOwned(type t) type {
  if isBorrowedClassType(t) {
    if isNilableClassType(t) {
      type tt = (t:owned class?);
      return tt;
    } else {
      type tt = (t:owned class);
      return tt;
    }
  }

  return t;
}

proc testToCq(type t) {
  var x = new t(1);
  writeln(x, " : ", x.type:string);
  var y = x:C?;
  writeln(" cast to C? -> ", y, " : ", y.type:string);
  if isUnmanagedClassType(t) {
    delete x;
  }
}
proc testToC(type t) {
  var x = new t(1);
  writeln(x, " : ", x.type:string);
  var y = x:C;
  writeln(" cast to C -> ", y, " : ", y.type:string);
  if isUnmanagedClassType(t) {
    delete x;
  }
}

proc testBorrowedToCq(type t) {
  type tt = borrowedToOwned(t);
  var alloc = new tt(1);
  var x = alloc.borrow();
  writeln(x, " : ", x.type:string);
  var y = x:C?;
  writeln(" cast to C? -> ", y, " : ", y.type:string);
}
proc testBorrowedToC(type t) {
  type tt = borrowedToOwned(t);
  var alloc = new tt(1);
  var x = alloc.borrow();
  writeln(x, " : ", x.type:string);
  var y = x:C;
  writeln(" cast to C -> ", y, " : ", y.type:string);
}

proc testValues() {
  testToCq(owned C );
  testToC (owned C?);
  testToCq(owned C?);
  testToC (owned C );

  testToCq(shared C );
  testToC (shared C?);
  testToCq(shared C?);
  testToC (shared C );

  testBorrowedToCq(borrowed C );
  testBorrowedToC (borrowed C?);
  testBorrowedToCq(borrowed C?);
  testBorrowedToC (borrowed C );

  testToCq(unmanaged C );
  testToC (unmanaged C?);
  testToCq(unmanaged C?);
  testToC (unmanaged C );
}
testValues();

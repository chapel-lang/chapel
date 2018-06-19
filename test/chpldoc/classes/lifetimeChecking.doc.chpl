/* Used to demonstrate the behavior of lifetime checking and documentation */
class Foo {
  var a: unmanaged Foo;
  var b: shared Foo;
  var c: owned Foo;
  var d: borrowed Foo;

  /* Validates when there is documentation */
  var w: borrowed Foo;
  /* Validates when there is documentation */
  var x: unmanaged Foo;
  /* Validates when there is documentation */
  var y: shared Foo;
  /* Validates when there is documentation */
  var z: owned Foo;

  proc takesOwned(x: owned Foo) { }

  proc takesShared(x: shared Foo) { }

  proc takesUnmanaged(x: unmanaged Foo) { }

  proc takesBorrowed(x: borrowed Foo) { }

  proc returnsOwned(): owned Foo { }

  proc returnsShared(): shared Foo { }

  proc returnsUnmanaged(): unmanaged Foo { }

  proc returnsBorrowed(): borrowed Foo { }

  proc mixNMatch(x: shared Foo): unmanaged Foo { }

  /* Validates when there is documentation */
  proc takesOwnedDoc(x: owned Foo) { }

  /* Validates when there is documentation */
  proc takesSharedDoc(x: shared Foo) { }

  /* Validates when there is documentation */
  proc takesUnmanagedDoc(x: unmanaged Foo) { }

  /* Validates when there is documentation */
  proc takesBorrowedDoc(x: borrowed Foo) { }

  /* Validates when there is documentation */
  proc returnsOwnedDoc(): owned Foo { }

  /* Validates when there is documentation */
  proc returnsSharedDoc(): shared Foo { }

  /* Validates when there is documentation */
  proc returnsUnmanagedDoc(): unmanaged Foo { }

  /* Validates when there is documentation */
  proc returnsBorrowedDoc(): borrowed Foo { }

  /* Validates when there is documentation */
  proc mixNMatchDoc(x: shared Foo): unmanaged Foo { }

}

proc takesOwned(x: owned Foo) { }

proc takesShared(x: shared Foo) { }

proc takesUnmanaged(x: unmanaged Foo) { }

proc takesBorrowed(x: borrowed Foo) { }

proc returnsOwned(): owned Foo { }

proc returnsShared(): shared Foo { }

proc returnsUnmanaged(): unmanaged Foo { }

proc returnsBorrowed(): borrowed Foo { }

proc mixNMatch(x: shared Foo): unmanaged Foo { }

/* Validates when there is documentation */
proc takesOwnedDoc(x: owned Foo) { }

/* Validates when there is documentation */
proc takesSharedDoc(x: shared Foo) { }

/* Validates when there is documentation */
proc takesUnmanagedDoc(x: unmanaged Foo) { }

/* Validates when there is documentation */
proc takesBorrowedDoc(x: borrowed Foo) { }

/* Validates when there is documentation */
proc returnsOwnedDoc(): owned Foo { }

/* Validates when there is documentation */
proc returnsSharedDoc(): shared Foo { }

/* Validates when there is documentation */
proc returnsUnmanagedDoc(): unmanaged Foo { }

/* Validates when there is documentation */
proc returnsBorrowedDoc(): borrowed Foo { }

/* Validates when there is documentation */
proc mixNMatchDoc(x: shared Foo): unmanaged Foo { }

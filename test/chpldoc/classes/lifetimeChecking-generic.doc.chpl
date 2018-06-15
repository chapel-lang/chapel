/* Generic type for use with all the lifetime checking styles */
class MyGeneric {
  type t;
}

/* Used to demonstrate the behavior of lifetime checking and documentation */
class Foo {
  var a: unmanaged MyGeneric(int);
  var b: shared MyGeneric(int);
  var c: owned MyGeneric(int);
  var d: borrowed MyGeneric(int);

  /* Validates when there is documentation */
  var w: borrowed MyGeneric(int);
  /* Validates when there is documentation */
  var x: unmanaged MyGeneric(int);
  /* Validates when there is documentation */
  var y: shared MyGeneric(int);
  /* Validates when there is documentation */
  var z: owned MyGeneric(int);

  proc takesOwned(x: owned MyGeneric(int)) { }

  proc takesShared(x: shared MyGeneric(int)) { }

  proc takesUnmanaged(x: unmanaged MyGeneric(int)) { }

  proc takesBorrowed(x: borrowed MyGeneric(int)) { }

  proc returnsOwned(): owned MyGeneric(int) { }

  proc returnsShared(): shared MyGeneric(int) { }

  proc returnsUnmanaged(): unmanaged MyGeneric(int) { }

  proc returnsBorrowed(): borrowed MyGeneric(int) { }

  proc mixNMatch(x: shared MyGeneric(int)): unmanaged MyGeneric(int) { }

  /* Validates when there is documentation */
  proc takesOwnedDoc(x: owned MyGeneric(int)) { }

  /* Validates when there is documentation */
  proc takesSharedDoc(x: shared MyGeneric(int)) { }

  /* Validates when there is documentation */
  proc takesUnmanagedDoc(x: unmanaged MyGeneric(int)) { }

  /* Validates when there is documentation */
  proc takesBorrowedDoc(x: borrowed MyGeneric(int)) { }

  /* Validates when there is documentation */
  proc returnsOwnedDoc(): owned MyGeneric(int) { }

  /* Validates when there is documentation */
  proc returnsSharedDoc(): shared MyGeneric(int) { }

  /* Validates when there is documentation */
  proc returnsUnmanagedDoc(): unmanaged MyGeneric(int) { }

  /* Validates when there is documentation */
  proc returnsBorrowedDoc(): borrowed MyGeneric(int) { }

  /* Validates when there is documentation */
  proc mixNMatchDoc(x: shared MyGeneric(int)): unmanaged MyGeneric(int) { }

}

proc takesOwned(x: owned MyGeneric(int)) { }

proc takesShared(x: shared MyGeneric(int)) { }

proc takesUnmanaged(x: unmanaged MyGeneric(int)) { }

proc takesBorrowed(x: borrowed MyGeneric(int)) { }

proc returnsOwned(): owned MyGeneric(int) { }

proc returnsShared(): shared MyGeneric(int) { }

proc returnsUnmanaged(): unmanaged MyGeneric(int) { }

proc returnsBorrowed(): borrowed MyGeneric(int) { }

proc mixNMatch(x: shared MyGeneric(int)): unmanaged MyGeneric(int) { }

/* Validates when there is documentation */
proc takesOwnedDoc(x: owned MyGeneric(int)) { }

/* Validates when there is documentation */
proc takesSharedDoc(x: shared MyGeneric(int)) { }

/* Validates when there is documentation */
proc takesUnmanagedDoc(x: unmanaged MyGeneric(int)) { }

/* Validates when there is documentation */
proc takesBorrowedDoc(x: borrowed MyGeneric(int)) { }

/* Validates when there is documentation */
proc returnsOwnedDoc(): owned MyGeneric(int) { }

/* Validates when there is documentation */
proc returnsSharedDoc(): shared MyGeneric(int) { }

/* Validates when there is documentation */
proc returnsUnmanagedDoc(): unmanaged MyGeneric(int) { }

/* Validates when there is documentation */
proc returnsBorrowedDoc(): borrowed MyGeneric(int) { }

/* Validates when there is documentation */
proc mixNMatchDoc(x: shared MyGeneric(int)): unmanaged MyGeneric(int) { }

/* Used to demonstrate the behavior of lifetime checking and documentation */
class Foo {
  var a: unmanaged;
  var b: shared;
  var c: owned;
  var d: borrowed;

  /* Validates when there is documentation */
  var w: unmanaged;
  /* Validates when there is documentation */
  var x: shared;
  /* Validates when there is documentation */
  var y: owned;
  /* Validates when there is documentation */
  var z: borrowed;

  proc takesOwned(x: owned) { }

  proc takesShared(x: shared) { }

  proc takesUnmanaged(x: unmanaged) { }

  proc takesBorrowed(x: borrowed) { }

  /* Validates when there is documentation */
  proc takesOwnedDoc(x: owned) { }

  /* Validates when there is documentation */
  proc takesSharedDoc(x: shared) { }

  /* Validates when there is documentation */
  proc takesUnmanagedDoc(x: unmanaged) { }

  /* Validates when there is documentation */
  proc takesBorrowedDoc(x: borrowed) { }
}

proc takesOwned(x: owned) { }

proc takesShared(x: shared) { }

proc takesUnmanaged(x: unmanaged) { }

proc takesBorrowed(x: borrowed) { }

/* Validates when there is documentation */
proc takesOwnedDoc(x: owned) { }

/* Validates when there is documentation */
proc takesSharedDoc(x: shared) { }

/* Validates when there is documentation */
proc takesUnmanagedDoc(x: unmanaged) { }

/* Validates when there is documentation */
proc takesBorrowedDoc(x: borrowed) { }
